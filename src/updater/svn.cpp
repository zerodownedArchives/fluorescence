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

#include "svn.hpp"

#include "misc/log.hpp"

#include <svncpp/status.hpp>
#include <svncpp/wc.hpp>
#include <svncpp/targets.hpp>

namespace fluo {
namespace updater {

SvnListener::SvnListener(SvnNotify* notify) : notify_(notify), cancel_(false) {
}

char SvnListener::actionToChar(const svn_wc_notify_action_t& action) const {
	switch (action) {
    case svn_wc_notify_add:						return 'A';break;
    case svn_wc_notify_delete:					return 'D';break;
    case svn_wc_notify_restore:					return 'R';break;
    case svn_wc_notify_revert:					return 'S';break;
    case svn_wc_notify_failed_revert:			return 's';break;
    case svn_wc_notify_copy:					return 'C';break;
    case svn_wc_notify_resolved:				return 'V';break;
    case svn_wc_notify_skip:					return '-';break;
    case svn_wc_notify_update_delete:			return 'D';break;
    case svn_wc_notify_update_add:				return 'A';break;
    case svn_wc_notify_update_update:			return 'U';break;
    case svn_wc_notify_update_completed:		return 'U';break;
    case svn_wc_notify_update_external:			return 'U';break;
    case svn_wc_notify_status_completed:		return 'S';break;
    case svn_wc_notify_status_external:			return 'S';break;
    case svn_wc_notify_commit_modified:			return '>';break;
    case svn_wc_notify_commit_deleted:			return '>';break;
    case svn_wc_notify_commit_added:			return '>';break;
    case svn_wc_notify_commit_replaced:			return '>';break;
    case svn_wc_notify_commit_postfix_txdelta:	return '>';break;

    case svn_wc_notify_blame_revision:			return 'B';break;
    case svn_wc_notify_locked:					return 'L';break;
    case svn_wc_notify_unlocked:				return 'N';break;
    case svn_wc_notify_failed_lock:				return 'l';break;
    case svn_wc_notify_failed_unlock:			return 'n';break;
    default:                                    return '?';break;
	}
}

bool SvnListener::contextGetLogin(const std::string &realm, std::string &username, std::string &password, bool &maySave) {
    username = "anonymous";
    password = "";
    maySave = false;

    return true;
}

void SvnListener::contextNotify(const char *path, svn_wc_notify_action_t action, svn_node_kind_t kind,
        const char *mime_type, svn_wc_notify_state_t content_state, svn_wc_notify_state_t prop_state, svn_revnum_t revision) {

    if (strlen(path) > 0) {
        std::stringstream sstr;
        sstr << actionToChar(action) << " " << path;
        LOG_INFO << sstr.str() << std::endl;

        if (notify_) {
            notify_->notify(sstr.str());
        }
    }
}

bool SvnListener::contextCancel() {
    return cancel_;
}

bool SvnListener::contextGetLogMessage(std::string &msg) {
    return false;
}

svn::ContextListener::SslServerTrustAnswer SvnListener::contextSslServerTrustPrompt(const svn::ContextListener::SslServerTrustData & data, apr_uint32_t & acceptedFailures){
    return svn::ContextListener::ACCEPT_TEMPORARILY;
}

bool SvnListener::contextSslClientCertPrompt(std::string & certFile){
    return false;
}

bool SvnListener::contextSslClientCertPwPrompt(std::string & password, const std::string & realm, bool & maySave){
    maySave = false;
    return false;
}

void SvnListener::cancel() {
    cancel_ = true;
}



Svn::Svn(const char* directory, const char* url, SvnNotify* notify) :
        directory_(directory), url_(url), svnListener_(notify), client_(&context_), notify_(notify) {
    context_.setListener(&svnListener_);
}

bool Svn::checkWorkingCopy() {
    return svn::Wc::checkWc(directory_);
}

int Svn::checkout() {
    try {
        svn_revnum_t rev = client_.checkout(url_, directory_, svn::Revision::HEAD, true);
        return rev;
    } catch (svn::ClientException& e) {
        LOG_ERROR << "Error at checkout: " << e.message() << std::endl;

        if (notify_) {
            notify_->notify("Error at checkout:");
            notify_->notify(e.message());
        }

        return -1;
    }
}

int Svn::getNewFileCount() {
    // does not really work, don't use!
    svn::StatusEntries entries;
    try {
        svn::Path path(directory_);
        entries = client_.status(path.c_str(), true, false, true, false, true);
    } catch (svn::ClientException& e) {
        LOG_ERROR << "Error at getNewFileCount: " << e.message() << std::endl;
        return -1;
    }

    svn::StatusEntries::const_iterator iter = entries.begin();
    svn::StatusEntries::const_iterator end = entries.end();

    LOG_DEBUG << "Check new files: " << entries.size() << std::endl;
    int newFileCount = 0;
    for (; iter != end; ++iter) {
        if (iter->textStatus() != iter->reposTextStatus() && iter->reposTextStatus() != svn_wc_status_none) {
            ++newFileCount;
            LOG_DEBUG << "entry: " << iter->path() << ": textStat=" << iter->textStatus() << " repoTextStat=" << iter->reposTextStatus() << std::endl;
        } else {
            LOG_DEBUG << "unversioned: " << iter->path() << ": textStat=" << iter->textStatus() << " repoTextStat=" << iter->reposTextStatus() << std::endl;
        }
    }

    return newFileCount;
}

int Svn::update() {
    try {
        // do cleanup first, to be safe
        client_.cleanup(directory_);
        svn_revnum_t rev = client_.update(svn::Path(directory_), svn::Revision::HEAD, true, true);
        return rev;
    } catch(svn::ClientException &e){
        LOG_ERROR << "Error at update: " << e.message() << std::endl;
        if (notify_) {
            notify_->notify("Error at update:");
            notify_->notify(e.message());
        }
        return -1;
    }
}

void Svn::cancel() {
    svnListener_.cancel();
}

bool Svn::revert() {
    try {
        // do cleanup first, to be safe
        client_.cleanup(directory_);
        client_.revert(svn::Targets(directory_), true);
        return true;
    } catch(svn::ClientException &e){
        LOG_ERROR << "Error at revert: " << e.message() << std::endl;
        if (notify_) {
            notify_->notify("Error at revert:");
            notify_->notify(e.message());
        }
        return false;
    }
}

}
}

