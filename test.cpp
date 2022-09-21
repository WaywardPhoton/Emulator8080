#include <check.h>
#include "instructions.h"

 //ADD A
 //-------------------------------------------
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

// SUB A
//-------------------------------------------
START_TEST(sub_reg_test)
{
    State theState;
    theState.A = 0xFF;
    theState.B = 0xFA;

    SUB_A(&theState, NULL, &theState.B, REG, false);

    ck_assert_int_eq(theState.A, (uint8_t) (0xFF - 0xFA));
}
END_TEST

START_TEST(sub_imm_test)
{
    State theState;
    theState.A = 0xFF;
    unsigned char instruction[] = {0xE2,0x82,0xAC, 0}; 
    SUB_A(&theState, instruction, NULL, IMM, false);
    ck_assert_int_eq(theState.A, (uint8_t) (0xFF - 0x82));

}
END_TEST

START_TEST(sub_addr_test)
{
    State theState;
    theState.A = 0xFF;
    theState.H = 0x01;
    theState.L = 0x02;
    
    theState.memory[0x102]= 0xFA ; 
    SUB_A(&theState, NULL, NULL, ADDR, false);
    ck_assert_int_eq(theState.A, (uint8_t) (0xFF - 0xFA));
}  
END_TEST

Suite *instruction_suite(void)
{
    Suite *s;
    TCase *tc_add;
    TCase *tc_sub;

    s = suite_create ("Instructions");
    tc_add = tcase_create ("Add");
    tc_sub = tcase_create ("Sub");

    tcase_add_test (tc_add, add_reg_test);
    tcase_add_test (tc_add, add_imm_test);
    tcase_add_test (tc_add, add_addr_test);

    tcase_add_test (tc_sub, sub_reg_test);
    tcase_add_test (tc_sub, sub_imm_test);
    tcase_add_test (tc_sub, sub_addr_test);

    //suite_add_tcase (s, tc_add);
    suite_add_tcase (s, tc_sub);
    
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