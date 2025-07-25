/*
 * Copyright 2013-2025, Corvusoft Ltd, All Rights Reserved.
 */

//System Includes
#include <regex>
#include <utility>
#include <ciso646>
#include <stdexcept>
#include <system_error>

//Project Includes
#include "corvusoft/restbed/uri.hpp"
#include "corvusoft/restbed/string.hpp"
#include "corvusoft/restbed/session.hpp"
#include "corvusoft/restbed/request.hpp"
#include "corvusoft/restbed/response.hpp"
#include "corvusoft/restbed/resource.hpp"
#include "corvusoft/restbed/settings.hpp"
#include "corvusoft/restbed/detail/socket_impl.hpp"
#include "corvusoft/restbed/detail/request_impl.hpp"
#include "corvusoft/restbed/detail/session_impl.hpp"
#include "corvusoft/restbed/detail/resource_impl.hpp"

//External Includes

//System Namespaces
using std::map;
using std::set;
using std::regex;
using std::smatch;
using std::string;
using std::getline;
using std::istream;
using std::function;
using std::multimap;
using std::make_pair;
using std::exception;
using std::to_string;
using std::ssub_match;
using std::shared_ptr;
using std::error_code;
using std::make_shared;
using std::regex_match;
using std::regex_error;
using std::runtime_error;
using std::placeholders::_1;
using std::rethrow_exception;
using std::current_exception;
using std::chrono::milliseconds;

//Project Namespaces
using restbed::detail::SessionImpl;

//External Namespaces
using asio::buffer;
using asio::streambuf;

namespace restbed
{
    namespace detail
    {
        SessionImpl::SessionImpl( void ) : m_id( "" ),
            m_request( nullptr ),
            m_resource( nullptr ),
            m_settings( nullptr ),
            m_web_socket_manager( nullptr ),
            m_headers( ),
            m_error_handler( nullptr ),
            m_keep_alive_callback( nullptr ),
            m_error_handler_invoked( false )
        {
            return;
        }
        
        SessionImpl::~SessionImpl( void )
        {
            return;
        }
        
        void SessionImpl::fetch_body( const size_t length, const shared_ptr< Session > session, const function< void ( const shared_ptr< Session >, const Bytes& ) >& callback ) const
        {
            Bytes data( length );
            asio::buffer_copy( asio::buffer( data ), session->m_pimpl->m_request->m_pimpl->m_buffer->data( ) );
            session->m_pimpl->m_request->m_pimpl->m_buffer->consume( length );
            
            auto& body = m_request->m_pimpl->m_body;
            
            if ( body.empty( ) )
            {
                body = data;
            }
            else
            {
                body.insert( body.end( ), data.begin( ), data.end( ) );
            }

            try
            {
                callback(session, data);
            }
            catch ( const int status_code )
            {
                const auto error_handler = session->m_pimpl->get_error_handler();
                error_handler( status_code, runtime_error( m_settings->get_status_message( status_code ) ), session );
            }
            catch ( const regex_error& re )
            {
                const auto error_handler = session->m_pimpl->get_error_handler();
                error_handler( 500, re, session );
            }
            catch ( const runtime_error& re )
            {
                const auto error_handler = session->m_pimpl->get_error_handler();
                error_handler( 400, re, session );
            }
            catch ( const exception& ex )
            {
                const auto error_handler = session->m_pimpl->get_error_handler();
                error_handler( 500, ex, session );
            }
            catch ( ... )
            {
                auto cex = current_exception( );

                if ( cex not_eq nullptr )
                {
                    try
                    {
                        rethrow_exception( cex );
                    }
                    catch ( const exception& ex )
                    {
                        const auto error_handler = session->m_pimpl->get_error_handler();
                        error_handler( 500, ex, session );
                    }
                    catch ( ... )
                    {
                        const auto error_handler = session->m_pimpl->get_error_handler();
                        error_handler( 500, runtime_error( "Internal Server Error" ), session );
                    }
                }
                else
                {
                    const auto error_handler = session->m_pimpl->get_error_handler();
                    error_handler( 500, runtime_error( "Internal Server Error" ), session );
                }
            }
        }
        
        void SessionImpl::transmit( const Response& response, const function< void ( const error_code&, size_t ) >& callback ) const
        {
            auto hdrs = m_settings->get_default_headers( );
            
            if ( m_resource not_eq nullptr )
            {
                const auto m_resource_headers = m_resource->m_pimpl->m_default_headers;
                hdrs.insert( m_resource_headers.begin( ), m_resource_headers.end( ) );
            }
            
            hdrs.insert( m_headers.begin( ), m_headers.end( ) );
            
            auto response_headers = response.get_headers( );
            hdrs.insert( response_headers.begin( ), response_headers.end( ) );
            
            auto payload = make_shared< Response >( );
            payload->set_headers( hdrs );
            payload->set_body( response.get_body( ) );
            payload->set_version( response.get_version( ) );
            payload->set_protocol( response.get_protocol( ) );
            payload->set_status_code( response.get_status_code( ) );
            payload->set_status_message( response.get_status_message( ) );
            
            if ( payload->get_status_message( ).empty( ) )
            {
                payload->set_status_message( m_settings->get_status_message( payload->get_status_code( ) ) );
            }
            
            m_request->m_pimpl->m_socket->start_write( to_bytes( payload ), callback );
        }
        
        const function< void ( const int, const exception&, const shared_ptr< Session > ) > SessionImpl::get_error_handler( void )
        {
            if ( m_error_handler_invoked )
            {
                return [ ]( const int, const exception&, const shared_ptr< Session > ) { };
            }
            
            m_error_handler_invoked = true;
            
            auto error_handler = ( m_resource not_eq nullptr and m_resource->m_pimpl->m_error_handler not_eq nullptr ) ? m_resource->m_pimpl->m_error_handler : m_error_handler;
            
            if ( error_handler == nullptr )
            {
                return [ ]( const int, const exception&, const shared_ptr< Session > session )
                {
                    if ( session not_eq nullptr and session->is_open( ) )
                    {
                        session->close( );
                    }
                };
            }
            
            return error_handler;
        }

        Bytes SessionImpl::to_bytes( const shared_ptr< Response >& value )
        {
            char* locale = nullptr;
            if (auto current_locale = setlocale( LC_NUMERIC, nullptr ) )
            {
                locale = strdup(current_locale);
                setlocale( LC_NUMERIC, "C" );
            }
            
            auto data = String::format( "%s/%.1f %i %s\r\n",
                                        value->get_protocol( ).data( ),
                                        value->get_version( ),
                                        value->get_status_code( ),
                                        value->get_status_message( ).data( ) );
            
            if (locale) {
                setlocale( LC_NUMERIC, locale );
                free( locale );
            }
            
            auto headers = value->get_headers( );
            
            if ( not headers.empty( ) )
            {
                data += String::join( headers, ": ", "\r\n" ) + "\r\n";
            }
            
            data += "\r\n";
            
            auto bytes = String::to_bytes( data );
            auto body = value->get_body( );
            
            if ( not body.empty( ) )
            {
                bytes.insert( bytes.end( ), body.begin( ), body.end( ) );
            }
            
            return bytes;
        }
    }
}
