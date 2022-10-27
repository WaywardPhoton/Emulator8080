#include <check.h>
#include "instructions.h"
#include <stdbool.h>

 //ADD A
 //-------------------------------------------
START_TEST(add_reg_test)
{
    State theState;
    theState.A = 0xFF;
    theState.B = 0xFA;
    theState.cc.cy = 1; 
    bool carry = true;
    ADD_A(&theState, &theState.B, REG, carry);

    ck_assert_int_eq(theState.A, (uint8_t) (0xFF + 0xFA + carry  ));
}
END_TEST

START_TEST(add_imm_test)
{
    State theState;
    theState.A = 0xFF;
    theState.cc.cy = 1; 
    theState.pc = 0;
    theState.memory[theState.pc+1]= 0x82;
    bool carry = true;
    ADD_A(&theState, NULL, IMM, carry);
    ck_assert_int_eq(theState.A, (uint8_t) (0xFF + 0x82 + carry));

}
END_TEST

START_TEST(add_addr_test)
{
    State theState;
    theState.A = 0xFF;
    theState.H = 0x01;
    theState.L = 0x02;
    theState.cc.cy = 1; 
    bool carry = true;
    theState.memory[0x102]= 0xFA ; 

    ADD_A(&theState, NULL, ADDR, carry);
    ck_assert_int_eq(theState.A, (uint8_t) (0xFF + 0xFA+ carry));
}  
END_TEST

// SUB A
//-------------------------------------------
START_TEST(sub_reg_test)
{
    State theState;
    theState.A = 0xFF;
    theState.B = 0xFA;

    theState.cc.cy = 1; 
    bool carry = true;

    SUB_A(&theState, &theState.B, REG, carry);

    ck_assert_int_eq(theState.A, (uint8_t) (0xFF - 0xFA - carry));
}
END_TEST

START_TEST(sub_imm_test)
{
    State theState;
    theState.A = 0xFF;
    theState.pc = 0;
    theState.memory[theState.pc+1]= 0x82;
    theState.cc.cy = 1; 
    bool carry = true;
    SUB_A(&theState, NULL, IMM, carry);
    ck_assert_int_eq(theState.A, (uint8_t) (0xFF - 0x82-carry));

}
END_TEST

START_TEST(sub_addr_test)
{
    State theState;
    theState.A = 0xFF;
    theState.H = 0x01;
    theState.L = 0x02;
    theState.cc.cy = 1; 
    bool carry = true;
    
    theState.memory[0x102]= 0xFA ; 
    SUB_A(&theState, NULL, ADDR, carry);
    ck_assert_int_eq(theState.A, (uint8_t) (0xFF - 0xFA-carry));
}  
END_TEST

// INX
START_TEST(inx_byte)
{
    State theState;
    theState.B = 0xFF;
    theState.C = 0x00;
    uint16_t intermediate; 
    INX(&theState, &theState.B, &theState.C, BYTE);
    intermediate= theState.read_reg(&theState.B, &theState.C);

    ck_assert_int_eq(intermediate,  0xFF01);

}
END_TEST

START_TEST(inx_word)
{
    State theState;
    theState.sp= 0xFFFA;
    uint16_t intermediate; 
    INX(&theState, NULL, NULL, WORD);
    

    ck_assert_int_eq(theState.sp,  0xFFFA +1);

}
END_TEST

Suite *instruction_suite(void)
{
    Suite *s;
    TCase *tc_add;
    TCase *tc_sub;
    TCase *tc_inx;

    s = suite_create ("Instructions");
    tc_add = tcase_create ("Add");
    tc_sub = tcase_create ("Sub");
    tc_inx = tcase_create ("Inx");

    tcase_add_test (tc_add, add_reg_test);
    tcase_add_test (tc_add, add_imm_test);
    tcase_add_test (tc_add, add_addr_test);

    tcase_add_test (tc_sub, sub_reg_test);
    tcase_add_test (tc_sub, sub_imm_test);
    tcase_add_test (tc_sub, sub_addr_test);

    tcase_add_test (tc_inx, inx_byte);
    tcase_add_test (tc_inx, inx_word);

    suite_add_tcase (s, tc_add);
    suite_add_tcase (s, tc_sub);
    suite_add_tcase (s, tc_inx);
    
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