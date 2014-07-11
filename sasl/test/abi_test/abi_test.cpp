#include <sasl/test/abi_test/abi_test.h>

#include <sasl/include/drivers/drivers_api.h>
#include <sasl/include/codegen/cg_api.h>
#include <sasl/include/semantic/symbol.h>
#include <sasl/include/semantic/semantics.h>
#include <sasl/include/common/diag_formatter.h>

#include <eflib/include/platform/boost_begin.h>
#include <boost/test/unit_test.hpp>
#include <eflib/include/platform/boost_end.h>

#include <fstream>

using namespace salviar;

void (*compiler_loader::create_compiler)( shared_ptr<compiler>& );

BOOST_GLOBAL_FIXTURE(compiler_loader);

BOOST_AUTO_TEST_SUITE(death);

BOOST_FIXTURE_TEST_CASE(semantic_fn_svs, abi_test_fixture)
{
	init_vs("repo/semantic_fn.svs");

	auto reflection2 = drv->get_reflection2();
	BOOST_REQUIRE(reflection2);

	BOOST_CHECK_EQUAL(reflection2->language(), salviar::lang_vertex_shader);
	BOOST_CHECK_EQUAL(reflection2->entry_name(), "Mfn@@");
	BOOST_CHECK_EQUAL(reflection2->available_reg_count(reg_categories::unknown), 0);
	BOOST_CHECK_EQUAL(reflection2->available_reg_count(reg_categories::offset), 0);
	BOOST_CHECK_EQUAL(reflection2->available_reg_count(reg_categories::outputs), 1);
	BOOST_CHECK_EQUAL(reflection2->available_reg_count(reg_categories::uniforms), 2);
	BOOST_CHECK_EQUAL(reflection2->available_reg_count(reg_categories::varying), 0);

	BOOST_CHECK( reflection2->varying_semantics().empty() );

	auto out_pos_reg = reflection2->find_reg( reg_categories::outputs, semantic_value("SV_Position", 0) );
	BOOST_CHECK( out_pos_reg.valid() );
	BOOST_CHECK(out_pos_reg.rfile.cat == reg_categories::outputs);
	BOOST_CHECK_EQUAL(out_pos_reg.rfile.index, 0);
	BOOST_CHECK_EQUAL(reflection2->reg_addr(out_pos_reg), 0);

	auto in_pos_reg = reflection2->find_reg( reg_categories::varying, semantic_value("SV_Position", 0) );
	BOOST_CHECK( !in_pos_reg.valid() );

	auto global_pos_reg = reflection2->find_reg( reg_categories::uniforms, semantic_value("SV_Position", 0) );
	BOOST_CHECK( global_pos_reg.valid() );
	BOOST_CHECK(global_pos_reg.rfile.cat == reg_categories::uniforms);
	BOOST_CHECK_EQUAL(global_pos_reg.rfile.index, rfile_name::global().index);
	BOOST_CHECK_EQUAL(reflection2->reg_addr(global_pos_reg), 0);
}

BOOST_AUTO_TEST_SUITE_END();