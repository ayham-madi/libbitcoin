/**
 * Copyright (c) 2011-2015 libwallet developers (see AUTHORS)
 *
 * This file is part of libwallet.
 *
 * libwallet is free software: you can redistribute it and/or modify
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
#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;
using namespace bc::wallet;

BOOST_AUTO_TEST_SUITE(hd_private_tests)

// TODO: test altchain

#define SHORT_SEED "000102030405060708090a0b0c0d0e0f"
#define LONG_SEED "fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542"

BOOST_AUTO_TEST_CASE(hd_private__encoded__round_trip__expected)
{
    static const auto encoded = "xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi";
    const hd_private key(encoded);
    BOOST_REQUIRE_EQUAL(key.encoded(), encoded);
}

BOOST_AUTO_TEST_CASE(hd_private__derive_private__short_seed__expected)
{
    data_chunk seed;
    BOOST_REQUIRE(decode_base16(seed, SHORT_SEED));

    const hd_private m(seed, hd_private::mainnet);
    const auto m0h = m.derive_private(hd_first_hardened_key);
    const auto m0h1 = m0h.derive_private(1);
    const auto m0h12h = m0h1.derive_private(2 + hd_first_hardened_key);
    const auto m0h12h2 = m0h12h.derive_private(2);
    const auto m0h12h2x = m0h12h2.derive_private(1000000000);

    BOOST_REQUIRE_EQUAL(m.encoded(), "xprv9s21ZrQH143K3QTDL4LXw2F7HEK3wJUD2nW2nRk4stbPy6cq3jPPqjiChkVvvNKmPGJxWUtg6LnF5kejMRNNU3TGtRBeJgk33yuGBxrMPHi");
    BOOST_REQUIRE_EQUAL(m0h.encoded(), "xprv9uHRZZhk6KAJC1avXpDAp4MDc3sQKNxDiPvvkX8Br5ngLNv1TxvUxt4cV1rGL5hj6KCesnDYUhd7oWgT11eZG7XnxHrnYeSvkzY7d2bhkJ7");
    BOOST_REQUIRE_EQUAL(m0h1.encoded(), "xprv9wTYmMFdV23N2TdNG573QoEsfRrWKQgWeibmLntzniatZvR9BmLnvSxqu53Kw1UmYPxLgboyZQaXwTCg8MSY3H2EU4pWcQDnRnrVA1xe8fs");
    BOOST_REQUIRE_EQUAL(m0h12h.encoded(), "xprv9z4pot5VBttmtdRTWfWQmoH1taj2axGVzFqSb8C9xaxKymcFzXBDptWmT7FwuEzG3ryjH4ktypQSAewRiNMjANTtpgP4mLTj34bhnZX7UiM");
    BOOST_REQUIRE_EQUAL(m0h12h2.encoded(), "xprvA2JDeKCSNNZky6uBCviVfJSKyQ1mDYahRjijr5idH2WwLsEd4Hsb2Tyh8RfQMuPh7f7RtyzTtdrbdqqsunu5Mm3wDvUAKRHSC34sJ7in334");
    BOOST_REQUIRE_EQUAL(m0h12h2x.encoded(), "xprvA41z7zogVVwxVSgdKUHDy1SKmdb533PjDz7J6N6mV6uS3ze1ai8FHa8kmHScGpWmj4WggLyQjgPie1rFSruoUihUZREPSL39UNdE3BBDu76");
}

BOOST_AUTO_TEST_CASE(hd_private__derive_private__long_seed__expected)
{
    data_chunk seed;
    BOOST_REQUIRE(decode_base16(seed, LONG_SEED));

    const hd_private m(seed, hd_private::mainnet);
    const auto m0 = m.derive_private(0);
    const auto m0xH = m0.derive_private(2147483647 + hd_first_hardened_key);
    const auto m0xH1 = m0xH.derive_private(1);
    const auto m0xH1yH = m0xH1.derive_private(2147483646 + hd_first_hardened_key);
    const auto m0xH1yH2 = m0xH1yH.derive_private(2);

    BOOST_REQUIRE_EQUAL(m.encoded(), "xprv9s21ZrQH143K31xYSDQpPDxsXRTUcvj2iNHm5NUtrGiGG5e2DtALGdso3pGz6ssrdK4PFmM8NSpSBHNqPqm55Qn3LqFtT2emdEXVYsCzC2U");
    BOOST_REQUIRE_EQUAL(m0.encoded(), "xprv9vHkqa6EV4sPZHYqZznhT2NPtPCjKuDKGY38FBWLvgaDx45zo9WQRUT3dKYnjwih2yJD9mkrocEZXo1ex8G81dwSM1fwqWpWkeS3v86pgKt");
    BOOST_REQUIRE_EQUAL(m0xH.encoded(), "xprv9wSp6B7kry3Vj9m1zSnLvN3xH8RdsPP1Mh7fAaR7aRLcQMKTR2vidYEeEg2mUCTAwCd6vnxVrcjfy2kRgVsFawNzmjuHc2YmYRmagcEPdU9");
    BOOST_REQUIRE_EQUAL(m0xH1.encoded(), "xprv9zFnWC6h2cLgpmSA46vutJzBcfJ8yaJGg8cX1e5StJh45BBciYTRXSd25UEPVuesF9yog62tGAQtHjXajPPdbRCHuWS6T8XA2ECKADdw4Ef");
    BOOST_REQUIRE_EQUAL(m0xH1yH.encoded(), "xprvA1RpRA33e1JQ7ifknakTFpgNXPmW2YvmhqLQYMmrj4xJXXWYpDPS3xz7iAxn8L39njGVyuoseXzU6rcxFLJ8HFsTjSyQbLYnMpCqE2VbFWc");
    BOOST_REQUIRE_EQUAL(m0xH1yH2.encoded(), "xprvA2nrNbFZABcdryreWet9Ea4LvTJcGsqrMzxHx98MMrotbir7yrKCEXw7nadnHM8Dq38EGfSh6dqA9QWTyefMLEcBYJUuekgW4BYPJcr9E7j");
}

BOOST_AUTO_TEST_SUITE_END()
