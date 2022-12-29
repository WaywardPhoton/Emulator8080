#include <check.h>
#include "CPU.h"
#include <stdbool.h>


 //ADD/ADC
 //-------------------------------------------
START_TEST(add_reg_test)
{
    CPU emulator;
    Memory memory; 
    memory.write(0x00, 0x88);
    uint16_t pc = 0x00;
    emulator.init(&memory, pc);

    emulator.state.A=0x14;
    emulator.state.B=0xF4;
    emulator.state.cc.cy= 1;

    emulator.step();
    State state = emulator.get_state();

    ck_assert_int_eq(state.A, (uint8_t) (0x14 + 0xF4 + 1));
    ck_assert_int_eq(state.cc.z, 0);
    ck_assert_int_eq(state.cc.cy, 1);
    ck_assert_int_eq(state.cc.s, 0);
    ck_assert_int_eq(state.cc.p, 1);


}
END_TEST

//ADI/ACI
START_TEST(add_imm_test)
{
    CPU emulator;
    Memory memory; 
    memory.write(0x00, 0xCE);
    memory.write(0x01, 0xF4);
    uint16_t pc = 0x00;
    emulator.init(&memory, pc);

    emulator.state.A=0x14;
    emulator.state.cc.cy= 1;

    emulator.step();
    State state = emulator.get_state();

    ck_assert_int_eq(state.A, (uint8_t) (0x14 + 0xF4 + 1));
    ck_assert_int_eq(state.cc.z, 0);
    ck_assert_int_eq(state.cc.cy, 1);
    ck_assert_int_eq(state.cc.s, 0);
    ck_assert_int_eq(state.cc.p, 1);

}
END_TEST

// SUB/SBB
START_TEST(sub_reg_test)
{
    CPU emulator;
    Memory memory; 
    memory.write(0x00, 0x98);
    uint16_t pc = 0x00;
    emulator.init(&memory, pc);

    emulator.state.A=0x14;
    emulator.state.B=0xF4;
    emulator.state.cc.cy= 1;
    
    emulator.step();
    State state = emulator.get_state();

    ck_assert_int_eq(state.A, (uint8_t) (0x14 - 0xF4 - 1));
    ck_assert_int_eq(state.cc.z, 0);
    ck_assert_int_eq(state.cc.cy, 1); //Carry is a borrow if it crosses -1
    ck_assert_int_eq(state.cc.s, 0);
    ck_assert_int_eq(state.cc.p, 0); //    00011111
}   
END_TEST

// SUI/SBI
START_TEST(sub_imm_test)
{
    CPU emulator;
    Memory memory; 
    memory.write(0x00, 0xDE);
    memory.write(0x01, 0xF4);
    uint16_t pc = 0x00;
    emulator.init(&memory, pc);

    emulator.state.A=0x14;
    emulator.state.cc.cy= 1;
    
    emulator.step();
    State state = emulator.get_state();

    ck_assert_int_eq(state.A, (uint8_t) (0x14 - 0xF4 - 1));
    ck_assert_int_eq(state.cc.z, 0);
    ck_assert_int_eq(state.cc.cy, 1); //Carry is a borrow if it crosses -1
    ck_assert_int_eq(state.cc.s, 0);
    ck_assert_int_eq(state.cc.p, 0); //    00011111

}
END_TEST

//INR
START_TEST(sub_inr_test)
{
    CPU emulator;
    Memory memory; 
    memory.write(0x00, 0x04);
    memory.write(0x01, 0xF4);
    uint16_t pc = 0x00;
    emulator.init(&memory, pc);

    emulator.state.B=0xfe;
    emulator.state.cc.cy= 0;
    
    emulator.step();
    State state = emulator.get_state();

    ck_assert_int_eq(state.B, (uint8_t) (0xfe +1));
    ck_assert_int_eq(state.cc.z, 0);
    ck_assert_int_eq(state.cc.cy, 0); //Carry not set
    ck_assert_int_eq(state.cc.s, 1);
    ck_assert_int_eq(state.cc.p, 1); //    
}
END_TEST

// DCR
START_TEST(sub_dcr_test)
{
    CPU emulator;
    Memory memory; 
    memory.write(0x00, 0x05);
    uint16_t pc = 0x00;
    emulator.init(&memory, pc);

    emulator.state.B=0x00;
    emulator.state.cc.cy= 0;
    
    emulator.step();
    State state = emulator.get_state();

    ck_assert_int_eq(state.B, (uint8_t) (0x00 -1));
    ck_assert_int_eq(state.cc.z, 0);
    ck_assert_int_eq(state.cc.cy, 0); // Carry not set
    ck_assert_int_eq(state.cc.s, 1);
    ck_assert_int_eq(state.cc.p, 1); //    
}
END_TEST


Suite *instruction_suite(void)
{
    Suite *s;
    TCase *tc_arithm;


    s = suite_create ("Instructions");
    tc_arithm= tcase_create ("Arithmetic");

    tcase_add_test (tc_arithm, add_reg_test);
    tcase_add_test (tc_arithm, add_imm_test);
    tcase_add_test (tc_arithm, sub_reg_test);
    tcase_add_test (tc_arithm, sub_imm_test);
    tcase_add_test (tc_arithm, sub_inr_test);
    tcase_add_test (tc_arithm, sub_dcr_test);


    suite_add_tcase (s, tc_arithm);

    
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