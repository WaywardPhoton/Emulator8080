#include <check.h>
#include "instructions.h"

START_TEST(add_reg_test)
{
    State theState;
    theState.A = 0xFF;
    theState.B = 0xFA;

    ADD_A(&theState, NULL, &theState.B, REG, false);

    ck_assert_int_eq(theState.A, (uint8_t) (0xFF + 0xFA));
}
END_TEST

START_TEST(add_imm_test)
{
    State theState;
    theState.A = 0xFF;
    unsigned char instruction[] = {0xE2,0x82,0xAC, 0}; 
    ADD_A(&theState, instruction, NULL, IMM, false);
    ck_assert_int_eq(theState.A, (uint8_t) (0xFF + 0x82));

}
END_TEST

START_TEST(add_addr_test)
{
    State theState;
    theState.A = 0xFF;
    theState.H = 0x01;
    theState.L = 0x02;
    
    theState.memory[0x102]= 0xFA ; 
    ADD_A(&theState, NULL, NULL, ADDR, false);
    ck_assert_int_eq(theState.A, (uint8_t) (0xFF + 0xFA));
}  
END_TEST

Suite *instruction_suite(void)
{
    Suite *s;
    TCase *tc_add;

    s = suite_create ("Instructions");
    tc_add = tcase_create ("Add");

    tcase_add_test (tc_add, add_reg_test);
    tcase_add_test (tc_add, add_imm_test);
    tcase_add_test (tc_add, add_addr_test);
    suite_add_tcase (s, tc_add);
    
    return s;
}

int main(void)
{
    Suite *s;
    SRunner *sr;

    s = instruction_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    
    return 0;
}