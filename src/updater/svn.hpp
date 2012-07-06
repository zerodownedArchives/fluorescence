/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef FLUO_UPDATER_SVN_HPP
#define FLUO_UPDATER_SVN_HPP

#include "svncpp/context.hpp"
#include "svncpp/client.hpp"
#include "svncpp/context_listener.hpp"

#include "svnnotify.hpp"

namespace fluo {
namespace updater {

class SvnListener : public svn::ContextListener {
public:
    SvnListener(SvnNotify* notify);

    virtual bool contextGetLogin(const std::string &realm, std::string &username, std::string &password, bool &maySave);
    virtual void contextNotify(const char *path, svn_wc_notify_action_t action, svn_node_kind_t kind, const char *mime_type, svn_wc_notify_state_t content_state, svn_wc_notify_state_t prop_state, svn_revnum_t revision);
    virtual bool contextCancel();
    virtual bool contextGetLogMessage(std::string &msg);
    virtual SslServerTrustAnswer contextSslServerTrustPrompt(const SslServerTrustData & data, apr_uint32_t & acceptedFailures);
    virtual bool contextSslClientCertPrompt(std::string & certFile);
    virtual bool contextSslClientCertPwPrompt(std::string & password, const std::string & realm, bool & maySave);

    void cancel();

private:
    char actionToChar(const svn_wc_notify_action_t& action) const;

    SvnNotify* notify_;

    bool cancel_;
};

class Svn {
public:
    Svn(const char* directory, const char* url, SvnNotify* notify);

    // check if the given directory is a valid working copy
    bool checkWorkingCopy();

    // returns the revision that was checked out, or -1 on error
    int checkout();

    // returns the revision that was checked out, or -1 on error
    int update();

    // returns -1 on error
    int getNewFileCount();

    // cancel the update or checkout process
    void cancel();

    // returns true on success
    bool revert();

    bool hasError() const;

private:
    const char* directory_;
    const char* url_;

    svn::Context context_;
	SvnListener svnListener_;
    svn::Client client_;

    SvnNotify* notify_;
};

}
}

#endif

