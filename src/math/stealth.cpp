/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/math/stealth.hpp>

#include <bitcoin/bitcoin/math/hash.hpp>
#include <bitcoin/bitcoin/chain/operation.hpp>
#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/utility/binary.hpp>

namespace libbitcoin {

using namespace chain;

bool shared_secret(ec_secret& out_shared, const ec_secret& secret,
    const ec_compressed& point)
{
    auto copy = point;
    if (!ec_multiply(copy, secret))
        return false;

    out_shared = sha256_hash(copy);
    return true;
}

bool uncover_stealth(ec_compressed& out_stealth,
    const ec_compressed& ephemeral, const ec_secret& scan,
    const ec_compressed& spend)
{
    ec_secret shared;
    if (!shared_secret(shared, scan, ephemeral))
        return false;

    auto copy = spend;
    if (!ec_add(copy, shared))
        return false;

    out_stealth = copy;
    return true;
}

bool uncover_stealth(ec_secret& out_stealth,
    const ec_compressed& ephemeral, const ec_secret& scan,
    const ec_secret& spend)
{
    ec_secret shared;
    if (!shared_secret(shared, scan, ephemeral))
        return false;

    auto copy = spend;
    if (!ec_add(copy, shared))
        return false;

    out_stealth = copy;
    return true;
}

static bool is_stealth_info(const script& script)
{
    if (script.type() != payment_type::stealth_info)
        return false;

    BITCOIN_ASSERT(script.operations.size() == 2);
    const auto& data = script.operations[1].data;
    return (data.size() >= hash_size);
}

bool to_stealth_prefix(binary_type& out_prefix, const script& script)
{
    if (!is_stealth_info(script))
        return false;

    // A stealth prefix is the full 32 bits (prefix to the hash).
    // A stealth filter is a leftmost substring of the stealth prefix.
    constexpr size_t size = binary_type::bits_per_block * sizeof(uint32_t);

    out_prefix = binary_type(size, bitcoin_hash(script.to_data(false)));
    return true;
}

bool to_ephemeral_key(ec_compressed& out_ephemeral_public_key,
    const script& script)
{
    if (!is_stealth_info(script))
        return false;

    // The sign of the ephemeral public key is fixed by convention.
    // This requires the spender to generate a compliant (y-even) key.
    // That requires iteration with probability of 1 in 2 chance of success.
    out_ephemeral_public_key[0] = ephemeral_public_key_sign;

    const auto& data = script.operations[1].data;
    std::copy(data.begin(), data.begin() + hash_size,
        out_ephemeral_public_key.begin() + 1);

    return true;
}

bool to_ephemeral_key(hash_digest& out_unsigned_ephemeral_key,
    const script& script)
{
    if (!is_stealth_info(script))
        return false;

    const auto& data = script.operations[1].data;
    std::copy(data.begin(), data.begin() + hash_size,
        out_unsigned_ephemeral_key.begin());

    return true;
}

} // namespace libbitcoin
