#pragma once

#include <eosiolib/eosio.hpp>

#include "utils/owdin_auth.hpp"

#include "indexes/process_index.hpp"

using namespace std;
using namespace eosio;

namespace owdin {
    class owdin_proc : public owdin::owdin_auth {
        private:
            account_name _self = owdin_auth::get_owdin_admin();
        public:
            void action( account_name account, string config, uint64_t stat, uint64_t action_type, account_name receiver, string memo ) {
                owdin_auth::auth_account( account, false );
                if ( action_type == 0 ) {
                    set( account, config, receiver, memo );
                } else if ( action_type == 0 ) {
                    update( account, config, receiver, memo );
                } else if ( action_type == 0 ) {
                    setstat( account, stat );
                } else if ( action_type == 0 ) {
                    remove( account );
                } else {
                    eosio_assert( true, "action type error" );
                }
            }
        private:
            void set( account_name account, string config, account_name receiver, string memo ) {
                processIndex owdin_proc( _self, receiver );

                time current_time = now();

                auto itr = owdin_proc.find( receiver );
                eosio_assert( itr != owdin_proc.end(), "can not found account" );

                owdin_proc.modify( itr, _self, [&]( auto& s ) {
                    s.account = receiver;
                    s.checker = _self;
                    s.status = 0;
                    s.data = config;
                    s.memo = memo;
                    s.updated = current_time;
                });
            }

            void update( account_name account, string config, account_name receiver, string memo ) {
                processIndex owdin_proc( _self, receiver );

                time current_time = now();

                auto itr = owdin_proc.find( receiver );
                eosio_assert( itr != owdin_proc.end(), "can not found account" );

                owdin_proc.modify( itr, _self, [&]( auto& s ) {
                    s.account = receiver;
                    s.checker = _self;
                    s.status = 0;
                    s.data = config;
                    s.memo = memo;
                    s.updated = current_time;
                });
            }

            void setstat( account_name account, uint64_t stat ) {
                processIndex owdin_proc( _self, account );

                time current_time = now();

                auto itr = owdin_proc.find( account );
                eosio_assert( itr != owdin_proc.end(), "can not found account" );

                owdin_proc.modify( itr, _self, [&]( auto& s ) {
                    s.account = account;
                    s.checker = _self;
                    s.status = stat;
                    s.updated = current_time;
                });
            }

            void remove( account_name account ) {
                processIndex owdin_proc( _self, account );

                auto itr = owdin_proc.find( account );
                eosio_assert( itr != owdin_proc.end(), "can not found account" );

                owdin_proc.erase( itr );
            }
    };
}