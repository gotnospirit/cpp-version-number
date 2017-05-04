#include <UnitTest++.h>

#include <Version.h>

TEST(VersionParse)
{
    CHECK(Version::Parse("..") == "");
    CHECK(Version::Parse("") == "0.0.0.0");
    CHECK(Version::Parse("a") == "0.0.0.0");

    CHECK(Version::Parse("a.b.c.d") == "0.0.0.0");

    CHECK(Version::Parse("1.b.c.d") == "0.0.0.0");
    CHECK(Version::Parse("a.2.c.d") == "0.0.0.0");
    CHECK(Version::Parse("a.b.3.d") == "0.0.0.0");
    CHECK(Version::Parse("a.b.c.4") == "0.0.0.0");

    CHECK(Version::Parse("1.2.c.d") == "0.0.0.0");
    CHECK(Version::Parse("1.2.3.d") == "0.0.0.0");

    CHECK(Version::Parse("1.2.3.4") == "1.2.3.4");
    CHECK(Version::Parse("1.0.3.4") == "1.0.3.4");

    CHECK(Version::Parse("1") == "1.0.0.0");
    CHECK(Version::Parse("1.2") == "1.2.0.0");
    CHECK(Version::Parse("1.2.3") == "1.2.3.0");

    CHECK(Version::Parse("1.2.3.4.5") == "1.2.3.4");
}

TEST(VersionCompare)
{
    {
        Version v1 { 1 };
        Version v2 { 0, 5 };

        CHECK_EQUAL(false, v1 == v2);
        CHECK_EQUAL(true, v1 != v2);
        CHECK_EQUAL(false, v1 < v2);
        CHECK_EQUAL(false, v1 <= v2);
        CHECK_EQUAL(true, v1 > v2);
        CHECK_EQUAL(true, v1 >= v2);
    }

    {
        Version v1 { 0, 5 };
        Version v2 { 1 };

        CHECK_EQUAL(false, v1 == v2);
        CHECK_EQUAL(true, v1 != v2);
        CHECK_EQUAL(true, v1 < v2);
        CHECK_EQUAL(true, v1 <= v2);
        CHECK_EQUAL(false, v1 > v2);
        CHECK_EQUAL(false, v1 >= v2);
    }

    {
        Version v1 { 1, 0, 2 };
        Version v2 { 1, 0, 2 };

        CHECK_EQUAL(true, v1 == v2);
        CHECK_EQUAL(false, v1 != v2);
        CHECK_EQUAL(false, v1 < v2);
        CHECK_EQUAL(true, v1 <= v2);
        CHECK_EQUAL(false, v1 > v2);
        CHECK_EQUAL(true, v1 >= v2);
    }

    {
        Version v1 { 1, 0, 2 };
        Version v2 { 1, 0, 2, 1 };

        CHECK_EQUAL(false, v1 == v2);
        CHECK_EQUAL(true, v1 != v2);
        CHECK_EQUAL(true, v1 < v2);
        CHECK_EQUAL(true, v1 <= v2);
        CHECK_EQUAL(false, v1 > v2);
        CHECK_EQUAL(false, v1 >= v2);
    }
}

TEST(VersionConvertToStdString)
{
    Version v { 0, 5 };

    CHECK_EQUAL(static_cast<std::string>(v), "0.5.0.0");

    std::string str_version = v;
    CHECK_EQUAL(str_version, "0.5.0.0");
}

TEST(VersionValidateInterval)
{
    CHECK_EQUAL(Version::ValidateInterval("abc"), false);
    CHECK_EQUAL(Version::ValidateInterval(","), false);
    CHECK_EQUAL(Version::ValidateInterval("-"), false);
    CHECK_EQUAL(Version::ValidateInterval("1.a"), false);
    CHECK_EQUAL(Version::ValidateInterval("1.a0"), false);
    CHECK_EQUAL(Version::ValidateInterval("1.0,"), false);
    CHECK_EQUAL(Version::ValidateInterval("1.0-"), false);
    CHECK_EQUAL(Version::ValidateInterval(",1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("-1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("{1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("1."), false);
    CHECK_EQUAL(Version::ValidateInterval(".0"), false);
    CHECK_EQUAL(Version::ValidateInterval(",1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("[.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("[[1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("[]1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("1].0"), false);
    CHECK_EQUAL(Version::ValidateInterval("1[.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("1..0"), false);
    CHECK_EQUAL(Version::ValidateInterval("1.0]-1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("[1.0]-1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("1.0[-1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("1.0-[1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("1.0-]1.0"), false);
    CHECK_EQUAL(Version::ValidateInterval("[-1"), false);
    CHECK_EQUAL(Version::ValidateInterval("]-1"), false);
    CHECK_EQUAL(Version::ValidateInterval("1-]"), false);
    CHECK_EQUAL(Version::ValidateInterval("1-["), false);

    CHECK_EQUAL(Version::ValidateInterval("1.0"), true);
    CHECK_EQUAL(Version::ValidateInterval("[1.0"), true);
    CHECK_EQUAL(Version::ValidateInterval("]1.0"), true);
    CHECK_EQUAL(Version::ValidateInterval("1.0]"), true);
    CHECK_EQUAL(Version::ValidateInterval("1.0["), true);
    CHECK_EQUAL(Version::ValidateInterval("1.0,1.0"), true);
    CHECK_EQUAL(Version::ValidateInterval("[1.0],[1.0]"), true);
    CHECK_EQUAL(Version::ValidateInterval("1.0-1.0"), true);
}

TEST(VersionIntervalMatch)
{
    // x < 4.2.0
    CHECK_EQUAL(Version::Parse("1.0").in("4.2.0["), true);
    CHECK_EQUAL(Version::Parse("4.2.0").in("4.2.0["), false);
    CHECK_EQUAL(Version::Parse("4.2.0.1").in("4.2.0["), false);
    CHECK_EQUAL(Version::Parse("5").in("4.2.0["), false);

    // x <= 4.2.0
    CHECK_EQUAL(Version::Parse("1.0").in("4.2.0]"), true);
    CHECK_EQUAL(Version::Parse("4.2.0").in("4.2.0]"), true);
    CHECK_EQUAL(Version::Parse("4.2.0.1").in("4.2.0]"), false);
    CHECK_EQUAL(Version::Parse("5").in("4.2.0]"), false);

    // x > 4.2.0
    CHECK_EQUAL(Version::Parse("1.0").in("]4.2.0"), false);
    CHECK_EQUAL(Version::Parse("4.1.9").in("]4.2.0"), false);
    CHECK_EQUAL(Version::Parse("4.2.0").in("]4.2.0"), false);
    CHECK_EQUAL(Version::Parse("4.2.0.1").in("]4.2.0"), true);
    CHECK_EQUAL(Version::Parse("5").in("]4.2.0"), true);

    // x >= 4.2.0
    CHECK_EQUAL(Version::Parse("1.0").in("[4.2.0"), false);
    CHECK_EQUAL(Version::Parse("4.2.0").in("[4.2.0"), true);
    CHECK_EQUAL(Version::Parse("4.2.0.1").in("[4.2.0"), true);
    CHECK_EQUAL(Version::Parse("5").in("[4.2.0"), true);

    // x == 4.2.0
    CHECK_EQUAL(Version::Parse("1.0").in("4.2.0"), false);
    CHECK_EQUAL(Version::Parse("4.2.0").in("4.2.0"), true);
    CHECK_EQUAL(Version::Parse("4.2.0.1").in("4.2.0"), false);
    CHECK_EQUAL(Version::Parse("5").in("4.2.0"), false);

    // x == 4.2.0
    CHECK_EQUAL(Version::Parse("1.0").in("[4.2.0]"), false);
    CHECK_EQUAL(Version::Parse("4.2.0").in("[4.2.0]"), true);
    CHECK_EQUAL(Version::Parse("4.2.0.1").in("[4.2.0]"), false);
    CHECK_EQUAL(Version::Parse("5").in("[4.2.0]"), false);

    // x != 4.2.0
    CHECK_EQUAL(Version::Parse("1.0").in("]4.2.0["), true);
    CHECK_EQUAL(Version::Parse("4.2.0").in("]4.2.0["), false);
    CHECK_EQUAL(Version::Parse("4.2.0.1").in("]4.2.0["), true);
    CHECK_EQUAL(Version::Parse("5").in("]4.2.0["), true);

    // x > 2.6.0 || x < 2.5
    CHECK_EQUAL(Version::Parse("2.4.9").in("]2.6,2.5["), true);
    CHECK_EQUAL(Version::Parse("2.5").in("]2.6,2.5["), false);
    CHECK_EQUAL(Version::Parse("2.6.0").in("]2.6,2.5["), false);
    CHECK_EQUAL(Version::Parse("2.6.1").in("]2.6,2.5["), true);

    // x in (2.0, 1.0)
    CHECK_EQUAL(Version::Parse("1").in("1.0,2.0"), true);
    CHECK_EQUAL(Version::Parse("1.1").in("1.0,2.0"), false);
    CHECK_EQUAL(Version::Parse("2.0").in("1.0,2.0"), true);

    // 2.6.0 >= x < 2.8
    CHECK_EQUAL(Version::Parse("1.0").in("[2.6.0-2.8["), false);
    CHECK_EQUAL(Version::Parse("2.5.9").in("[2.6.0-2.8["), false);
    CHECK_EQUAL(Version::Parse("2.6.0").in("[2.6.0-2.8["), true);
    CHECK_EQUAL(Version::Parse("2.7.9").in("[2.6.0-2.8["), true);
    CHECK_EQUAL(Version::Parse("2.8").in("[2.6.0-2.8["), false);
    CHECK_EQUAL(Version::Parse("5").in("[2.6.0-2.8["), false);

    // Intervalle invalide
    CHECK_EQUAL(Version::Parse("2.6.0").in("[2.6.0]-2.8["), false);
}

int main()
{
    return UnitTest::RunAllTests();
}

#undef EVAL