/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file Test.cpp
 * @authors:
 *   Dimitry Khokhlov <dimitry@ethdev.com>
 * @date 2016
 */

#include "Test.h"
#include "BuildInfo.h"
#include <jsonrpccpp/common/errors.h>
#include <jsonrpccpp/common/exception.h>
#include <libdevcore/CommonJS.h>
#include <libethcore/Common.h>
#include <libethereum/ChainParams.h>
#include <libethereum/ClientTest.h>
#include <libethereum/Transaction.h>
#include <libweb3jsonrpc/JsonHelper.h>

using namespace std;
using namespace dev;
using namespace dev::rpc;
using namespace jsonrpc;

Test::Test(eth::Client& _eth): m_eth(_eth) {}

string prepareVersionString()
{
    // cpp-1.3.0+commit.6be76b64.Linux.g++
    string commit(DEV_QUOTED(ETH_COMMIT_HASH));
    string version = "cpp-" + string(ETH_PROJECT_VERSION);
    version += "+commit." + commit.substr(0, 8);
    version +=
        "." + string(DEV_QUOTED(ETH_BUILD_OS)) + "." + string(DEV_QUOTED(ETH_BUILD_COMPILER));
    return version;
}

string Test::test_getClientInfo()
{
    return prepareVersionString();
}

Json::Value fillJsonWithState(eth::State const& _state, eth::AccountMaskMap const& _map)
{
    bool mapEmpty = (_map.size() == 0);
    Json::Value oState;
    for (auto const& a : _state.addresses())
    {
        if (_map.size() && _map.find(a.first) == _map.end())
            continue;

        Json::Value o;
        if (mapEmpty || _map.at(a.first).hasBalance())
            o["balance"] = toCompactHexPrefixed(_state.balance(a.first), 1);
        if (mapEmpty || _map.at(a.first).hasNonce())
            o["nonce"] = toCompactHexPrefixed(_state.getNonce(a.first), 1);

        if (mapEmpty || _map.at(a.first).hasStorage())
        {
            Json::Value store;
            for (auto const& s : _state.storage(a.first))
                store[toCompactHexPrefixed(s.second.first, 1)] =
                    toCompactHexPrefixed(s.second.second, 1);
            o["storage"] = store;
        }

        if (mapEmpty || _map.at(a.first).hasCode())
        {
            if (_state.code(a.first).size() > 0)
                o["code"] = toHexPrefixed(_state.code(a.first));
            else
                o["code"] = "";
        }
        oState[toHexPrefixed(a.first)] = o;
    }
    return oState;
}

string exportLog(eth::LogEntries const& _logs)
{
    RLPStream s;
    s.appendList(_logs.size());
    for (eth::LogEntry const& l : _logs)
        l.streamRLP(s);
    return toHexPrefixed(sha3(s.out()));
}

const int c_postStateJustHashVersion = 1;
const int c_postStateFullStateVersion = 2;
const int c_postStateLogHashVersion = 3;
string Test::test_getPostState(Json::Value const& param1)
{
    Json::Value out;
    Json::FastWriter fastWriter;
    if (u256(param1["version"].asString()) == c_postStateJustHashVersion)
        return toJS(m_eth.blockChain().info().stateRoot());
    else if (u256(param1["version"].asString()) == c_postStateFullStateVersion)
    {
        eth::AccountMaskMap _map;
        out = fillJsonWithState(m_eth.postState().state(), _map);
        return fastWriter.write(out);
    }
    else if (u256(param1["version"].asString()) == c_postStateLogHashVersion)
    {
		if (m_eth.blockChain().receipts().receipts.size() != 0)
			return exportLog(m_eth.blockChain().receipts().receipts[0].log());
		else
            return toJS(EmptyListSHA3);
    }
    return "";
}

string Test::test_addTransaction(Json::Value const& param1)
{
    try
    {
        eth::TransactionSkeleton tr = eth::toTransactionSkeleton(param1);
        m_eth.submitTransaction(tr, Secret(param1["secretKey"].asString()));
    }
    catch (...)
    {
        BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INVALID_PARAMS));
    }

    return "";
}

bool Test::test_setChainParams(Json::Value const& param1)
{
	try
	{
		Json::FastWriter fastWriter;
		std::string output = fastWriter.write(param1);
		asClientTest(m_eth).setChainParams(output);
	}
	catch (std::exception const&)
	{
		BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
	}

	return true;
}

bool Test::test_mineBlocks(int _number)
{
	try
	{
		asClientTest(m_eth).mineBlocks(_number);
	}
	catch (std::exception const&)
	{
		BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
	}

	return true;
}

bool Test::test_modifyTimestamp(int _timestamp)
{
	// FIXME: Fix year 2038 issue.
	try
	{
		asClientTest(m_eth).modifyTimestamp(_timestamp);
	}
	catch (std::exception const&)
	{
		BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
	}
	return true;
}

bool Test::test_addBlock(std::string const& _rlp)
{
	try
	{
		asClientTest(m_eth).addBlock(_rlp);
	}
	catch (std::exception const&)
	{
		BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
	}
	return true;
}

bool Test::test_rewindToBlock(int _number)
{
	try
	{
		m_eth.rewind(_number);
		asClientTest(m_eth).completeSync();
	}
	catch (std::exception const&)
	{
		BOOST_THROW_EXCEPTION(JsonRpcException(Errors::ERROR_RPC_INTERNAL_ERROR));
	}
	return true;
}
