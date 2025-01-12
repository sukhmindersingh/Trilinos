#include <gtest/gtest.h>
#include <stk_util/environment/OptionsSpecification.hpp>

namespace {

TEST(OptionsSpecification, construct_empty)
{
    stk::OptionsSpecification optionsSpec;

    EXPECT_TRUE(optionsSpec.empty());
}

TEST(OptionsSpecification, insert_flag_and_print)
{
    stk::OptionsSpecification optionsSpec;

    optionsSpec.add_options()("flag,f", "this is a flag");

    EXPECT_EQ(1u, optionsSpec.size());
    EXPECT_FALSE(optionsSpec.empty());

    std::ostringstream os;
    os << optionsSpec;
    std::ostringstream oss;
    oss<<"--flag,-f   this is a flag"<<std::endl<<std::endl;
    std::string expected = oss.str();
    EXPECT_EQ(expected, os.str());
}

TEST(OptionsSpecification, insert_required_option_and_print)
{
    stk::OptionsSpecification optionsSpec;

    const bool isFlag = true;
    const bool isRequired = true;
    optionsSpec.add_options()("option,o", isFlag, isRequired, "this is a required option");

    EXPECT_EQ(1u, optionsSpec.size());
    EXPECT_FALSE(optionsSpec.empty());

    std::ostringstream os;
    os << optionsSpec;
    std::ostringstream oss;
    oss<<"--option,-o   this is a required option (required)"<<std::endl<<std::endl;
    std::string expected = oss.str();
    EXPECT_EQ(expected, os.str());
}

TEST(OptionsSpecification, insert_option_with_default_value_and_print)
{
    stk::OptionsSpecification optionsSpec;

    optionsSpec.add_options()("option,o", "this is an option", stk::DefaultValue<double>(99.9));

    EXPECT_EQ(1u, optionsSpec.size());
    EXPECT_FALSE(optionsSpec.empty());

    std::ostringstream os;
    os << optionsSpec;
    std::ostringstream oss;
    oss<<"--option,-o   this is an option default: 99.9"<<std::endl<<std::endl;
    std::string expected = oss.str();
    EXPECT_EQ(expected, os.str());
}

TEST(OptionsSpecification, insert_option_with_implicit_value_and_print)
{
    stk::OptionsSpecification optionsSpec;

    optionsSpec.add_options()("option,o", "this is an option", stk::ImplicitValue<double>(99.9));

    EXPECT_EQ(1u, optionsSpec.size());
    EXPECT_FALSE(optionsSpec.empty());

    std::ostringstream os;
    os << optionsSpec;
    std::ostringstream oss;
    oss<<"--option,-o   this is an option implicit: 99.9"<<std::endl<<std::endl;
    std::string expected = oss.str();
    EXPECT_EQ(expected, os.str());
}

TEST(OptionsSpecification, insert_flag_and_option_with_default_value_and_print)
{
    stk::OptionsSpecification optionsSpec;

    optionsSpec.add_options()("flag,f", "this is a flag");
    optionsSpec.add_options()("option,o", "this is an option", stk::DefaultValue<double>(99.9));

    EXPECT_EQ(2u, optionsSpec.size());
    EXPECT_FALSE(optionsSpec.empty());

    std::ostringstream os;
    os << optionsSpec;
    std::ostringstream oss;
    oss<<"--flag,-f     this is a flag"<<std::endl;
    oss<<"--option,-o   this is an option default: 99.9"<<std::endl<<std::endl;
    std::string expected = oss.str();
    EXPECT_EQ(expected, os.str());
}

TEST(OptionsSpecification, print_sub_options)
{
    stk::OptionsSpecification optionsSpec("Main Options");

    optionsSpec.add_options()("flag,f", "this is a flag");
    optionsSpec.add_options()("option,o", "this is an option", stk::DefaultValue<double>(99.9));

    {
      stk::OptionsSpecification subOptionsSpec("Sub Options");

      subOptionsSpec.add_options()("input-file,i", "input file");
      subOptionsSpec.add_options()("log-file,l", "log file");

      optionsSpec.add(subOptionsSpec);
    }

    const stk::Option& option = optionsSpec.find_option("input-file");
    EXPECT_FALSE(option.name.empty());

    std::ostringstream os;
    os << optionsSpec;
    std::ostringstream oss;
    oss<<"Main Options\n"<<std::endl;
    oss<<"--flag,-f     this is a flag"<<std::endl;
    oss<<"--option,-o   this is an option default: 99.9"<<std::endl<<std::endl;
    oss<<"Sub Options\n"<<std::endl;
    oss<<"--input-file,-i   input file"<<std::endl;
    oss<<"--log-file,-l     log file"<<std::endl<<std::endl;
    std::string expected = oss.str();
    EXPECT_EQ(expected, os.str());
}

}
