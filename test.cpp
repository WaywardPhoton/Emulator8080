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

// DCX
START_TEST(dcx_byte)
{
    State theState;
    theState.B = 0xFF;
    theState.C = 0x00;
    uint16_t intermediate; 
    DCX(&theState, &theState.B, &theState.C, BYTE);
    intermediate= theState.read_reg(&theState.B, &theState.C);

    ck_assert_int_eq(intermediate,  0xFEFF);

}
END_TEST

START_TEST(dcx_word)
{
    State theState;
    theState.sp= 0xFFFA;
    uint16_t intermediate; 
    DCX(&theState, NULL, NULL, WORD);
    

    ck_assert_int_eq(theState.sp,  0xFFFA -1);

}
END_TEST

// INR
START_TEST(inr_byte)
{
    State theState;
    theState.B = 0xFF;
    INR(&theState, &theState.B, BYTE);
    ck_assert_int_eq(theState.B,  0x00);

}
END_TEST

START_TEST(inr_word)
{
    State theState;
    theState.H=0xFC;
    theState.L = 0x02;
    theState.memory[0xFC02] = 0xFC;
    INR(&theState, NULL, WORD);
    ck_assert_int_eq(theState.memory[0xFC02],  0xFD);

}
END_TEST

// DCR
START_TEST(dcr_byte)
{
    State theState;
    theState.B = 0xFF;
    DCR(&theState, &theState.B, BYTE);
    ck_assert_int_eq(theState.B,  0xFE);

}
END_TEST

START_TEST(dcr_word)
{
    State theState;
    theState.H=0xFC;
    theState.L = 0x02;
    theState.memory[0xFC02] = 0xFC;
    DCR(&theState, NULL, WORD);
    ck_assert_int_eq(theState.memory[0xFC02],  0xFB);

}
END_TEST

// DAD

START_TEST(dad)
{
    State theState;
    theState.H=0xFC;
    theState.L = 0x02;
    theState.D=0xFE;
    theState.E = 0x0F;
    DAD(&theState, &(theState.D), &(theState.E));
    uint16_t reg16 = theState.read_reg(&theState.H, &theState.L);
    ck_assert_int_eq(theState.cc.cy,  1);

}
END_TEST

START_TEST(dad_sp)
{
    State theState;
    theState.H=0xFC;
    theState.L = 0x02;
    theState.sp=0xFE0F;

    DAD_SP(&theState);
    uint16_t reg16 = theState.read_reg(&theState.H, &theState.L);
    ck_assert_int_eq(reg16,  0xFA11);

}
END_TEST

START_TEST(jnz)
{
    State theState;
    theState.memory[0]= 0x10;
    theState.memory[1]= 0x20;
    theState.memory[2]= 0xAF;
    theState.pc = 0x00;
    JNZ(&theState);
    ck_assert_int_eq(theState.pc,  0xAF20);

}
END_TEST

START_TEST(jz)
{
    State theState;
    theState.cc.z = 1;
    theState.memory[0]= 0x10;
    theState.memory[1]= 0x20;
    theState.memory[2]= 0xAF;
    theState.pc = 0x00;
    JZ(&theState);
    ck_assert_int_eq(theState.pc,  0xAF20);

}
END_TEST



Suite *instruction_suite(void)
{
    Suite *s;
    TCase *tc_add;
    TCase *tc_sub;
    TCase *tc_inx;
    TCase *tc_dcx;
    TCase *tc_inr;
    TCase *tc_dcr;
    TCase *tc_dad;
    TCase *tc_jnz;


    s = suite_create ("Instructions");
    tc_add = tcase_create ("Add");
    tc_sub = tcase_create ("Sub");
    tc_inx = tcase_create ("Inx");
    tc_dcx = tcase_create ("Dcx");
    tc_inr = tcase_create ("Inr");
    tc_dcr = tcase_create ("Dcr");
    tc_dad = tcase_create ("Dad");
    tc_jnz = tcase_create ("Jnz");

    tcase_add_test (tc_add, add_reg_test);
    tcase_add_test (tc_add, add_imm_test);
    tcase_add_test (tc_add, add_addr_test);

    tcase_add_test (tc_sub, sub_reg_test);
    tcase_add_test (tc_sub, sub_imm_test);
    tcase_add_test (tc_sub, sub_addr_test);

    tcase_add_test (tc_inx, inx_byte);
    tcase_add_test (tc_inx, inx_word);
    tcase_add_test (tc_dcx, dcx_byte);
    tcase_add_test (tc_dcx, dcx_word);

    tcase_add_test (tc_inr, inr_byte);
    tcase_add_test (tc_inr, inr_word);
    tcase_add_test (tc_dcr, dcr_byte);
    tcase_add_test (tc_dcr, dcr_word);

    tcase_add_test (tc_dad, dad);
    tcase_add_test (tc_dad, dad_sp);
    tcase_add_test (tc_jnz, jnz);



    suite_add_tcase (s, tc_add);
    suite_add_tcase (s, tc_sub);
    suite_add_tcase (s, tc_inx);
    suite_add_tcase (s, tc_dcx);
    suite_add_tcase (s, tc_inr);
    suite_add_tcase (s, tc_dcr);
    suite_add_tcase (s, tc_dad);
    suite_add_tcase (s, tc_jnz);
    
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