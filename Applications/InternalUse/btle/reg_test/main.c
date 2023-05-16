/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Id: main.c 41625 2019-03-12 21:24:18Z kevin.gillespie $
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "/home/kgills/Workspace/cordio-pan-software/branches/sdma_refactor/controller/sources/dbb/prot/ble/pan2g5/afe/max32665/include/afe_regs.h"
#include "/home/kgills/Workspace/cordio-pan-software/branches/sdma_refactor/controller/sources/dbb/prot/ble/pan2g5/include/dbb_ctrl_regs.h"
#include "/home/kgills/Workspace/cordio-pan-software/branches/sdma_refactor/controller/sources/dbb/prot/ble/pan2g5/include/dbb_tx_regs.h"
#include "/home/kgills/Workspace/cordio-pan-software/branches/sdma_refactor/controller/sources/dbb/prot/ble/pan2g5/include/dbb_rx_regs.h"
#include "/home/kgills/Workspace/cordio-pan-software/branches/sdma_refactor/controller/sources/dbb/prot/ble/pan2g5/include/dbb_rffe_regs.h"

#define AFE_BASE_ADDR                (0x40001100)
#define DBB_CTRL_BASE_ADDR           (0x40001200)
#define DBB_TX_BASE_ADDR             (0x40001300)
#define DBB_RX_BASE_ADDR             (0x40001400)
#define DBB_RFFE_BASE_ADDR           (0x40001500)

// Function stubs for MAXQ compiler
void BTLE_IRQHandler(void) {}
void TMR0_IRQHandler(void) {}

/******************************************************************************/
int test_afe(void)
{
    mxc_afe_regs_t* afe_base = (mxc_afe_regs_t*)AFE_BASE_ADDR;

    if((uint32_t)&(afe_base->reg1) != AFE_BASE_ADDR + MXC_R_AFE_REG1) {
        printf("Error with reg1\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg2) != AFE_BASE_ADDR + MXC_R_AFE_REG2) {
        printf("Error with reg2\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg3) != AFE_BASE_ADDR + MXC_R_AFE_REG3) {
        printf("Error with reg3\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg4) != AFE_BASE_ADDR + MXC_R_AFE_REG4) {
        printf("Error with reg4\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg5) != AFE_BASE_ADDR + MXC_R_AFE_REG5) {
        printf("Error with reg5\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg6) != AFE_BASE_ADDR + MXC_R_AFE_REG6) {
        printf("Error with reg6\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg7) != AFE_BASE_ADDR + MXC_R_AFE_REG7) {
        printf("Error with reg7\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg8) != AFE_BASE_ADDR + MXC_R_AFE_REG8) {
        printf("Error with reg8\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg9) != AFE_BASE_ADDR + MXC_R_AFE_REG9) {
        printf("Error with reg9\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg10) != AFE_BASE_ADDR + MXC_R_AFE_REG10) {
        printf("Error with reg10\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg11) != AFE_BASE_ADDR + MXC_R_AFE_REG11) {
        printf("Error with reg11\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg12) != AFE_BASE_ADDR + MXC_R_AFE_REG12) {
        printf("Error with reg12\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg13) != AFE_BASE_ADDR + MXC_R_AFE_REG13) {
        printf("Error with reg13\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg14) != AFE_BASE_ADDR + MXC_R_AFE_REG14) {
        printf("Error with reg14\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->reg15) != AFE_BASE_ADDR + MXC_R_AFE_REG15) {
        printf("Error with reg15\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega1) != AFE_BASE_ADDR + MXC_R_AFE_REGA1) {
        printf("Error with rega1\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega2) != AFE_BASE_ADDR + MXC_R_AFE_REGA2) {
        printf("Error with rega2\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega3) != AFE_BASE_ADDR + MXC_R_AFE_REGA3) {
        printf("Error with rega3\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega4) != AFE_BASE_ADDR + MXC_R_AFE_REGA4) {
        printf("Error with rega4\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega5) != AFE_BASE_ADDR + MXC_R_AFE_REGA5) {
        printf("Error with rega5\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega6) != AFE_BASE_ADDR + MXC_R_AFE_REGA6) {
        printf("Error with rega6\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega7) != AFE_BASE_ADDR + MXC_R_AFE_REGA7) {
        printf("Error with rega7\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega8) != AFE_BASE_ADDR + MXC_R_AFE_REGA8) {
        printf("Error with rega8\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega9) != AFE_BASE_ADDR + MXC_R_AFE_REGA9) {
        printf("Error with rega9\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega10) != AFE_BASE_ADDR + MXC_R_AFE_REGA10) {
        printf("Error with rega10\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega11) != AFE_BASE_ADDR + MXC_R_AFE_REGA11) {
        printf("Error with rega11\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega12) != AFE_BASE_ADDR + MXC_R_AFE_REGA12) {
        printf("Error with rega12\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega13) != AFE_BASE_ADDR + MXC_R_AFE_REGA13) {
        printf("Error with rega13\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega14) != AFE_BASE_ADDR + MXC_R_AFE_REGA14) {
        printf("Error with rega14\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega15) != AFE_BASE_ADDR + MXC_R_AFE_REGA15) {
        printf("Error with rega15\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->rega16) != AFE_BASE_ADDR + MXC_R_AFE_REGA16) {
        printf("Error with rega16\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->regb1) != AFE_BASE_ADDR + MXC_R_AFE_REGB1) {
        printf("Error with regb1\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->regb2) != AFE_BASE_ADDR + MXC_R_AFE_REGB2) {
        printf("Error with regb2\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->regb3) != AFE_BASE_ADDR + MXC_R_AFE_REGB3) {
        printf("Error with regb3\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->regb4) != AFE_BASE_ADDR + MXC_R_AFE_REGB4) {
        printf("Error with regb4\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->regb5) != AFE_BASE_ADDR + MXC_R_AFE_REGB5) {
        printf("Error with regb5\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->regb6) != AFE_BASE_ADDR + MXC_R_AFE_REGB6) {
        printf("Error with regb6\r\n");
        return 0;
    }
    if((uint32_t)&(afe_base->regb7) != AFE_BASE_ADDR + MXC_R_AFE_REGB7) {
        printf("Error with regb7\r\n");
        return 0;
    }

    return 1;
}

/******************************************************************************/
int test_dbb_ctrl(void)
{
    mxc_dbb_ctrl_regs_t* dbb_ctrl_base = (mxc_dbb_ctrl_regs_t*)DBB_CTRL_BASE_ADDR;

    if((uint32_t)&(dbb_ctrl_base->dbb_ctrl_version) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_DBB_CTRL_VERSION) {
        printf("Error with dbb_ctrl_version\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->dbb_ctrl_rst) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_DBB_CTRL_RST) {
        printf("Error with dbb_ctrl_rst\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->tx_ctrl_dbb_en_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_TX_CTRL_DBB_EN_DLY) {
        printf("Error with tx_ctrl_dbb_en_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->tx_ctrl_rffe_en_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_TX_CTRL_RFFE_EN_DLY) {
        printf("Error with tx_ctrl_rffe_en_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->tx_ctrl_dbb_dis_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_TX_CTRL_DBB_DIS_DLY) {
        printf("Error with tx_ctrl_dbb_dis_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->tx_ctrl_rffe_dis_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_TX_CTRL_RFFE_DIS_DLY) {
        printf("Error with tx_ctrl_rffe_dis_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->tx_ctrl_cmd) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_TX_CTRL_CMD) {
        printf("Error with tx_ctrl_cmd\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->tx_ctrl_debug_en_tx_on_sfd_to) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_TX_CTRL_DEBUG_EN_TX_ON_SFD_TO) {
        printf("Error with tx_ctrl_debug_en_tx_on_sfd_to\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->rx_ctrl_dbb_en_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_RX_CTRL_DBB_EN_DLY) {
        printf("Error with rx_ctrl_dbb_en_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->rx_ctrl_rffe_en_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_RX_CTRL_RFFE_EN_DLY) {
        printf("Error with rx_ctrl_rffe_en_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->rx_ctrl_dbb_dis_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_RX_CTRL_DBB_DIS_DLY) {
        printf("Error with rx_ctrl_dbb_dis_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->rx_ctrl_rffe_dis_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_RX_CTRL_RFFE_DIS_DLY) {
        printf("Error with rx_ctrl_rffe_dis_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->rx_ctrl_cmd) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_RX_CTRL_CMD) {
        printf("Error with rx_ctrl_cmd\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->tx_pmu_wake_up_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_TX_PMU_WAKE_UP_DLY) {
        printf("Error with tx_pmu_wake_up_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->tx_pmu_ctrl) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_TX_PMU_CTRL) {
        printf("Error with tx_pmu_ctrl\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->rffe_pmu_wake_up_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_RFFE_PMU_WAKE_UP_DLY) {
        printf("Error with rffe_pmu_wake_up_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->rffe_pmu_ctrl) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_RFFE_PMU_CTRL) {
        printf("Error with rffe_pmu_ctrl\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->rx_pmu_wake_up_dly) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_RX_PMU_WAKE_UP_DLY) {
        printf("Error with rx_pmu_wake_up_dly\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->rx_pmu_ctrl) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_RX_PMU_CTRL) {
        printf("Error with rx_pmu_ctrl\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->gen_pmu_status) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_GEN_PMU_STATUS) {
        printf("Error with gen_pmu_status\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_cntr_clk_mult_p) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_CNTR_CLK_MULT_P) {
        printf("Error with event_timing_cntr_clk_mult_p\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_cntr_clk_div_q) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_CNTR_CLK_DIV_Q) {
        printf("Error with event_timing_cntr_clk_div_q\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_cntr_val) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_CNTR_VAL) {
        printf("Error with event_timing_cntr_val\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_tx_enable_time) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_TX_ENABLE_TIME) {
        printf("Error with event_timing_tx_enable_time\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_rx_enable_time) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_RX_ENABLE_TIME) {
        printf("Error with event_timing_rx_enable_time\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_gp_event_time) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_GP_EVENT_TIME) {
        printf("Error with event_timing_gp_event_time\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_tx_enable_delta_time) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_TX_ENABLE_DELTA_TIME) {
        printf("Error with event_timing_tx_enable_delta_time\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_rx_enable_delta_time) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_RX_ENABLE_DELTA_TIME) {
        printf("Error with event_timing_rx_enable_delta_time\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_gp_event_delta_time) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_GP_EVENT_DELTA_TIME) {
        printf("Error with event_timing_gp_event_delta_time\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_ctrl) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_CTRL) {
        printf("Error with event_timing_ctrl\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_timestamp_tx_done) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_TIMESTAMP_TX_DONE) {
        printf("Error with event_timing_timestamp_tx_done\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_timestamp_rx_received) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_TIMESTAMP_RX_RECEIVED) {
        printf("Error with event_timing_timestamp_rx_received\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_timestamp_rx_sfd_det) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_TIMESTAMP_RX_SFD_DET) {
        printf("Error with event_timing_timestamp_rx_sfd_det\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_timestamp_rx_sfd_to) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_TIMESTAMP_RX_SFD_TO) {
        printf("Error with event_timing_timestamp_rx_sfd_to\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->event_timing_timestamp_rx_energy_det) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENT_TIMING_TIMESTAMP_RX_ENERGY_DET) {
        printf("Error with event_timing_timestamp_rx_energy_det\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->events_status) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENTS_STATUS) {
        printf("Error with events_status\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->events_irq_test) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_EVENTS_IRQ_TEST) {
        printf("Error with events_irq_test\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->cmu_gating_on) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_CMU_GATING_ON) {
        printf("Error with cmu_gating_on\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->cmu_main_mult_p) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_CMU_MAIN_MULT_P) {
        printf("Error with cmu_main_mult_p\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->cmu_main_div_q) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_CMU_MAIN_DIV_Q) {
        printf("Error with cmu_main_div_q\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->cmu_phy_mult_p) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_CMU_PHY_MULT_P) {
        printf("Error with cmu_phy_mult_p\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->cmu_phy_div_q) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_CMU_PHY_DIV_Q) {
        printf("Error with cmu_phy_div_q\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->cmu_dl_mult_p) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_CMU_DL_MULT_P) {
        printf("Error with cmu_dl_mult_p\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_ctrl_base->cmu_dl_div_q) != DBB_CTRL_BASE_ADDR + MXC_R_DBB_CTRL_CMU_DL_DIV_Q) {
        printf("Error with cmu_dl_div_q\r\n");
        return 0;
    }
    return 1;
}

/******************************************************************************/
int test_dbb_tx(void)
{
    mxc_dbb_tx_regs_t* dbb_tx_base = (mxc_dbb_tx_regs_t*)DBB_TX_BASE_ADDR;

    if((uint32_t)&(dbb_tx_base->tx_info_version) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_INFO_VERSION) {
        printf("Error with tx_info_version\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_general_standard) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_GENERAL_STANDARD) {
        printf("Error with tx_general_standard\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_general_control) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_GENERAL_CONTROL) {
        printf("Error with tx_general_control\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_general_send_multi) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_GENERAL_SEND_MULTI) {
        printf("Error with tx_general_send_multi\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_high_freq_coef) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_HIGH_FREQ_COEF) {
        printf("Error with tx_phy_high_freq_coef\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_low_freq_coef) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_LOW_FREQ_COEF) {
        printf("Error with tx_phy_low_freq_coef\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_freq_carrier) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_FREQ_CARRIER) {
        printf("Error with tx_phy_freq_carrier\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_amp_coef) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_AMP_COEF) {
        printf("Error with tx_phy_amp_coef\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_filt) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_FILT) {
        printf("Error with tx_phy_filt\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_filt_coefs) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_FILT_COEFS) {
        printf("Error with tx_phy_filt_coefs\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_freq_offs) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_FREQ_OFFS) {
        printf("Error with tx_phy_freq_offs\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_ovrsamp_ratio) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_OVRSAMP_RATIO) {
        printf("Error with tx_phy_ovrsamp_ratio\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_pattern_gen) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_PATTERN_GEN) {
        printf("Error with tx_phy_pattern_gen\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_pattern) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_PATTERN) {
        printf("Error with tx_phy_pattern\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_const_output) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_CONST_OUTPUT) {
        printf("Error with tx_phy_const_output\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_const_output_amp) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_CONST_OUTPUT_AMP) {
        printf("Error with tx_phy_const_output_amp\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_const_output_freq_high) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_CONST_OUTPUT_FREQ_HIGH) {
        printf("Error with tx_phy_const_output_freq_high\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_phy_const_output_freq_low) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PHY_CONST_OUTPUT_FREQ_LOW) {
        printf("Error with tx_phy_const_output_freq_low\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dbg_dl_lb) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DBG_DL_LB) {
        printf("Error with tx_dbg_dl_lb\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_bypass) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_BYPASS) {
        printf("Error with tx_dl_bypass\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_crc_mode) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_CRC_MODE) {
        printf("Error with tx_dl_crc_mode\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_crc_init_phr) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_CRC_INIT_PHR) {
        printf("Error with tx_dl_crc_init_phr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_crc_init_pld) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_CRC_INIT_PLD) {
        printf("Error with tx_dl_crc_init_pld\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_bch) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_BCH) {
        printf("Error with tx_dl_bch\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_ban_settings) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_BAN_SETTINGS) {
        printf("Error with tx_dl_ban_settings\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_btle_settings_adv_acc_addr) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_BTLE_SETTINGS_ADV_ACC_ADDR) {
        printf("Error with tx_dl_btle_settings_adv_acc_addr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_btle_settings_acc_addr) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_BTLE_SETTINGS_ACC_ADDR) {
        printf("Error with tx_dl_btle_settings_acc_addr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_btle_settings_whit) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_BTLE_SETTINGS_WHIT) {
        printf("Error with tx_dl_btle_settings_whit\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_btle_longrange) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_BTLE_LONGRANGE) {
        printf("Error with tx_dl_btle_longrange\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_phr_modulation) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_PHR_MODULATION) {
        printf("Error with tx_dl_phr_modulation\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl_phr) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL_PHR) {
        printf("Error with tx_dl_phr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_pld_mem) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_PLD_MEM) {
        printf("Error with tx_pld_mem\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_tx_base->tx_dl2_btle_speed_mode) != DBB_TX_BASE_ADDR + MXC_R_DBB_TX_TX_DL2_BTLE_SPEED_MODE) {
        printf("Error with tx_dl2_btle_speed_mode\r\n");
        return 0;
    }
   
    return 1;
}

/******************************************************************************/
int test_dbb_rx(void)
{
    mxc_dbb_rx_regs_t* dbb_rx_base = (mxc_dbb_rx_regs_t*)DBB_RX_BASE_ADDR;

    if((uint32_t)&(dbb_rx_base->rx_info_version) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_INFO_VERSION) {
        printf("Error with rx_info_version\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_general_standard) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_GENERAL_STANDARD) {
        printf("Error with rx_general_standard\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_general_control) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_GENERAL_CONTROL) {
        printf("Error with rx_general_control\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_general_control) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_GENERAL_CONTROL) {
        printf("Error with rx_phy_general_control\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_general_agc_freeze_timer) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_GENERAL_AGC_FREEZE_TIMER) {
        printf("Error with rx_phy_general_agc_freeze_timer\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_general_sig_detect_init_timer) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_GENERAL_SIG_DETECT_INIT_TIMER) {
        printf("Error with rx_phy_general_sig_detect_init_timer\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_general_sig_detect_rst_timer) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_GENERAL_SIG_DETECT_RST_TIMER) {
        printf("Error with rx_phy_general_sig_detect_rst_timer\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_general_ddc_rst_timer) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_GENERAL_DDC_RST_TIMER) {
        printf("Error with rx_phy_general_ddc_rst_timer\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_ddc_alpha_start) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_DDC_ALPHA_START) {
        printf("Error with rx_phy_ddc_alpha_start\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_ddc_alpha_stop) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_DDC_ALPHA_STOP) {
        printf("Error with rx_phy_ddc_alpha_stop\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_ddc_alpha_step) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_DDC_ALPHA_STEP) {
        printf("Error with rx_phy_ddc_alpha_step\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_ddc_beta) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_DDC_BETA) {
        printf("Error with rx_phy_ddc_beta\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_ddc_rst_tmr) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_DDC_RST_TMR) {
        printf("Error with rx_phy_ddc_rst_tmr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_ddc_offs_i) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_DDC_OFFS_I) {
        printf("Error with rx_phy_ddc_offs_i\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_ddc_offs_q) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_DDC_OFFS_Q) {
        printf("Error with rx_phy_ddc_offs_q\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_overwr) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_OVERWR) {
        printf("Error with rx_phy_agc5_overwr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_gain) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_GAIN) {
        printf("Error with rx_phy_agc5_gain\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh7upper) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH7UPPER) {
        printf("Error with rx_phy_agc5_thresh7upper\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh7lower) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH7LOWER) {
        printf("Error with rx_phy_agc5_thresh7lower\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh6upper) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH6UPPER) {
        printf("Error with rx_phy_agc5_thresh6upper\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh6lower) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH6LOWER) {
        printf("Error with rx_phy_agc5_thresh6lower\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh5upper) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH5UPPER) {
        printf("Error with rx_phy_agc5_thresh5upper\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh5lower) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH5LOWER) {
        printf("Error with rx_phy_agc5_thresh5lower\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh4upper) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH4UPPER) {
        printf("Error with rx_phy_agc5_thresh4upper\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh4lower) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH4LOWER) {
        printf("Error with rx_phy_agc5_thresh4lower\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh3upper) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH3UPPER) {
        printf("Error with rx_phy_agc5_thresh3upper\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh3lower) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH3LOWER) {
        printf("Error with rx_phy_agc5_thresh3lower\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh2upper) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH2UPPER) {
        printf("Error with rx_phy_agc5_thresh2upper\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh2lower) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH2LOWER) {
        printf("Error with rx_phy_agc5_thresh2lower\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh1upper) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH1UPPER) {
        printf("Error with rx_phy_agc5_thresh1upper\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh1lower) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH1LOWER) {
        printf("Error with rx_phy_agc5_thresh1lower\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh0upper) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH0UPPER) {
        printf("Error with rx_phy_agc5_thresh0upper\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_thresh0lower) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_THRESH0LOWER) {
        printf("Error with rx_phy_agc5_thresh0lower\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_row_upper) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_ROW_UPPER) {
        printf("Error with rx_phy_agc5_row_upper\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_row_lower) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_ROW_LOWER) {
        printf("Error with rx_phy_agc5_row_lower\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_cntr) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_CNTR) {
        printf("Error with rx_phy_agc5_cntr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_agc5_offs_init) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_AGC5_OFFS_INIT) {
        printf("Error with rx_phy_agc5_offs_init\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_phsmagest_filter_coeff) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_PHSMAGEST_FILTER_COEFF) {
        printf("Error with rx_phy_phsmagest_filter_coeff\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_phsmagest_scaling_factor) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_PHSMAGEST_SCALING_FACTOR) {
        printf("Error with rx_phy_phsmagest_scaling_factor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_filters_coefficients) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_FILTERS_COEFFICIENTS) {
        printf("Error with rx_phy_filters_coefficients\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_filters_settings) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_FILTERS_SETTINGS) {
        printf("Error with rx_phy_filters_settings\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_rssi_settings) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_RSSI_SETTINGS) {
        printf("Error with rx_phy_rssi_settings\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_rssi_ed_threshold) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_RSSI_ED_THRESHOLD) {
        printf("Error with rx_phy_rssi_ed_threshold\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det_settings) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET_SETTINGS) {
        printf("Error with rx_phy_signal_det_settings\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det_thresholds) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET_THRESHOLDS) {
        printf("Error with rx_phy_signal_det_thresholds\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det_select_v2) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET_SELECT_V2) {
        printf("Error with rx_phy_signal_det_select_v2\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det_settings_v2_div_factor) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET_SETTINGS_V2_DIV_FACTOR) {
        printf("Error with rx_phy_signal_det_settings_v2_div_factor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det_gear_v2) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET_GEAR_V2) {
        printf("Error with rx_phy_signal_det_gear_v2\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det_thresholds_v2_noise_factor) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET_THRESHOLDS_V2_NOISE_FACTOR) {
        printf("Error with rx_phy_signal_det_thresholds_v2_noise_factor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_frame_sync_pd_comp) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_FRAME_SYNC_PD_COMP) {
        printf("Error with rx_phy_frame_sync_pd_comp\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_frame_sync_sfd) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_FRAME_SYNC_SFD) {
        // This is a 64 bit field, but it's not 64-bit aligned.
        printf("Error with rx_phy_frame_sync_sfd = 0x%x\r\n", (uint32_t)&(dbb_rx_base->rx_phy_frame_sync_sfd));
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_frame_sync_settings) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_FRAME_SYNC_SETTINGS) {
        printf("Error with rx_phy_frame_sync_settings\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_frame_sync_settings_min_corr_val) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_FRAME_SYNC_SETTINGS_MIN_CORR_VAL) {
        printf("Error with rx_phy_frame_sync_settings_min_corr_val\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_frame_sync_timeout_tmr_periode) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_FRAME_SYNC_TIMEOUT_TMR_PERIODE) {
        printf("Error with rx_phy_frame_sync_timeout_tmr_periode\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_timing_sync_cfg) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIMING_SYNC_CFG) {
        printf("Error with rx_phy_timing_sync_cfg\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_timing_sync_log_sum) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIMING_SYNC_LOG_SUM) {
        printf("Error with rx_phy_timing_sync_log_sum\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_tim_sync_old_cfg) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIM_SYNC_OLD_CFG) {
        printf("Error with rx_phy_tim_sync_old_cfg\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_tim_sync_old_te_thresh) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIM_SYNC_OLD_TE_THRESH) {
        printf("Error with rx_phy_tim_sync_old_te_thresh\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_tim_sync_old_te_thresh_scaling) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIM_SYNC_OLD_TE_THRESH_SCALING) {
        printf("Error with rx_phy_tim_sync_old_te_thresh_scaling\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_tim_sync_old_init) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIM_SYNC_OLD_INIT) {
        printf("Error with rx_phy_tim_sync_old_init\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_tim_sync_old_te_cnt_thresh) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIM_SYNC_OLD_TE_CNT_THRESH) {
        printf("Error with rx_phy_tim_sync_old_te_cnt_thresh\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_tim_sync_old_te_cnt) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIM_SYNC_OLD_TE_CNT) {
        printf("Error with rx_phy_tim_sync_old_te_cnt\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_tim_sync_old_te_fifo_max_ptr) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIM_SYNC_OLD_TE_FIFO_MAX_PTR) {
        printf("Error with rx_phy_tim_sync_old_te_fifo_max_ptr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_tim_sync_old_adjust_cnt) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_TIM_SYNC_OLD_ADJUST_CNT) {
        printf("Error with rx_phy_tim_sync_old_adjust_cnt\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_cfg) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_CFG) {
        printf("Error with rx_phy_cfo_cfg\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_filter_coefficients) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_FILTER_COEFFICIENTS) {
        printf("Error with rx_phy_cfo_filter_coefficients\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_filter_scaling) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_FILTER_SCALING) {
        printf("Error with rx_phy_cfo_filter_scaling\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_pa_precnt_num) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_PA_PRECNT_NUM) {
        printf("Error with rx_phy_cfo_pa_precnt_num\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_ph_accum_part_num) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_PH_ACCUM_PART_NUM) {
        printf("Error with rx_phy_cfo_ph_accum_part_num\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_ed_thresh_factor) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_ED_THRESH_FACTOR) {
        printf("Error with rx_phy_cfo_ed_thresh_factor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_ed_slow_factor) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_ED_SLOW_FACTOR) {
        printf("Error with rx_phy_cfo_ed_slow_factor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_ed_fast_factor) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_ED_FAST_FACTOR) {
        printf("Error with rx_phy_cfo_ed_fast_factor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_pa_max_limit_val) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_PA_MAX_LIMIT_VAL) {
        printf("Error with rx_phy_cfo_pa_max_limit_val\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_cfo_est) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_CFO_EST) {
        printf("Error with rx_phy_cfo_cfo_est\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo_cfo_est_khz) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO_CFO_EST_KHZ) {
        printf("Error with rx_phy_cfo_cfo_est_khz\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_debug_dl_lb) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DEBUG_DL_LB) {
        printf("Error with rx_debug_dl_lb\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_dbg_cfg) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_DBG_CFG) {
        printf("Error with rx_phy_dbg_cfg\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_dbg_data) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_DBG_DATA) {
        printf("Error with rx_phy_dbg_data\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_bypass) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_BYPASS) {
        printf("Error with rx_dl_bypass\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_crc_mode) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_CRC_MODE) {
        printf("Error with rx_dl_crc_mode\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_crc_init_phr) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_CRC_INIT_PHR) {
        printf("Error with rx_dl_crc_init_phr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_crc_init_pld) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_CRC_INIT_PLD) {
        printf("Error with rx_dl_crc_init_pld\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_crc_residu) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_CRC_RESIDU) {
        printf("Error with rx_dl_crc_residu\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_bch) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_BCH) {
        printf("Error with rx_dl_bch\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_sprd_seq_err_nr) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_SPRD_SEQ_ERR_NR) {
        printf("Error with rx_dl_sprd_seq_err_nr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_ban_settings) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_BAN_SETTINGS) {
        printf("Error with rx_dl_ban_settings\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_btle_settings_adv_acc_addr) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_BTLE_SETTINGS_ADV_ACC_ADDR) {
        printf("Error with rx_dl_btle_settings_adv_acc_addr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_btle_settings_whit) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_BTLE_SETTINGS_WHIT) {
        printf("Error with rx_dl_btle_settings_whit\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_phr_modulation) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_PHR_MODULATION) {
        printf("Error with rx_dl_phr_modulation\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_rssi_out_pwr) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_RSSI_OUT_PWR) {
        printf("Error with rx_phy_rssi_out_pwr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_out_crc_stat) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_OUT_CRC_STAT) {
        printf("Error with rx_dl_out_crc_stat\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_out_phr) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_OUT_PHR) {
        printf("Error with rx_dl_out_phr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_out_acc_add_nok) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_OUT_ACC_ADD_NOK) {
        printf("Error with rx_dl_out_acc_add_nok\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_dl_out_ci) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_DL_OUT_CI) {
        printf("Error with rx_dl_out_ci\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_pld_mem) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PLD_MEM) {
        printf("Error with rx_pld_mem\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_timeout_tmr_periode) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_TIMEOUT_TMR_PERIODE) {
        printf("Error with rx_phy_signal_det2_timeout_tmr_periode\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_ed_slow_fact) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_ED_SLOW_FACT) {
        printf("Error with rx_phy_signal_det2_ed_slow_fact\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_ed_fast_fact) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_ED_FAST_FACT) {
        printf("Error with rx_phy_signal_det2_ed_fast_fact\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_ed_threshold_fact) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_ED_THRESHOLD_FACT) {
        printf("Error with rx_phy_signal_det2_ed_threshold_fact\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_ed_timeout_tmr_periode) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_ED_TIMEOUT_TMR_PERIODE) {
        printf("Error with rx_phy_signal_det2_ed_timeout_tmr_periode\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_stop_high_filt_corner) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_STOP_HIGH_FILT_CORNER) {
        printf("Error with rx_phy_signal_det2_stop_high_filt_corner\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_beta_mux_en_stage) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_BETA_MUX_EN_STAGE) {
        printf("Error with rx_phy_signal_det2_beta_mux_en_stage\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_beta_mux_threshold) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_BETA_MUX_THRESHOLD) {
        printf("Error with rx_phy_signal_det2_beta_mux_threshold\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_gamma_mux_en_stage) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_GAMMA_MUX_EN_STAGE) {
        printf("Error with rx_phy_signal_det2_gamma_mux_en_stage\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_gamma_mux_threshold) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_GAMMA_MUX_THRESHOLD) {
        printf("Error with rx_phy_signal_det2_gamma_mux_threshold\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_alpha) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_ALPHA) {
        printf("Error with rx_phy_signal_det2_alpha\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_beta) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_BETA) {
        printf("Error with rx_phy_signal_det2_beta\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_signal_det2_gamma) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_SIGNAL_DET2_GAMMA) {
        printf("Error with rx_phy_signal_det2_gamma\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo2_ed_timeout_tmr_periode) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO2_ED_TIMEOUT_TMR_PERIODE) {
        printf("Error with rx_phy_cfo2_ed_timeout_tmr_periode\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo2_cfg_ph_accum_upscl_fact) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO2_CFG_PH_ACCUM_UPSCL_FACT) {
        printf("Error with rx_phy_cfo2_cfg_ph_accum_upscl_fact\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo2_cfg_ph_accum_dwnscl_fact) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO2_CFG_PH_ACCUM_DWNSCL_FACT) {
        printf("Error with rx_phy_cfo2_cfg_ph_accum_dwnscl_fact\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_cfo2_cfg_ph_accum_comp) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_CFO2_CFG_PH_ACCUM_COMP) {
        printf("Error with rx_phy_cfo2_cfg_ph_accum_comp\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_btlespeed_anti_alias_enable) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_BTLESPEED_ANTI_ALIAS_ENABLE) {
        printf("Error with rx_phy_btlespeed_anti_alias_enable\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_btlespeed_filter_coeff) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_BTLESPEED_FILTER_COEFF) {
        printf("Error with rx_phy_btlespeed_filter_coeff\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_btlespeed_scaling_factor) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_BTLESPEED_SCALING_FACTOR) {
        printf("Error with rx_phy_btlespeed_scaling_factor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_frame_sync_lr_corr_length) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_FRAME_SYNC_LR_CORR_LENGTH) {
        printf("Error with rx_phy_frame_sync_lr_corr_length\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rx_base->rx_phy_frame_sync_lr_thresh_factor) != DBB_RX_BASE_ADDR + MXC_R_DBB_RX_RX_PHY_FRAME_SYNC_LR_THRESH_FACTOR) {
        printf("Error with rx_phy_frame_sync_lr_thresh_factor\r\n");
        return 0;
    }

    return 1;
}


/******************************************************************************/
int test_dbb_rffe(void)
{
    mxc_dbb_rffe_regs_t* dbb_rffe_base = (mxc_dbb_rffe_regs_t*)DBB_RFFE_BASE_ADDR;

    if((uint32_t)&(dbb_rffe_base->rffe_ifc_version) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RFFE_IFC_VERSION) {
        printf("Error with rffe_ifc_version\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rffe_ifc_rffe_version) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RFFE_IFC_RFFE_VERSION) {
        printf("Error with rffe_ifc_rffe_version\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->general_param) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_GENERAL_PARAM) {
        printf("Error with general_param\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->general_delay_freq) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_GENERAL_DELAY_FREQ) {
        printf("Error with general_delay_freq\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->general_delay_amp) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_GENERAL_DELAY_AMP) {
        printf("Error with general_delay_amp\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->general_spi_invert_csn) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_GENERAL_SPI_INVERT_CSN) {
        printf("Error with general_spi_invert_csn\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->general_skip_sdmod) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_GENERAL_SKIP_SDMOD) {
        printf("Error with general_skip_sdmod\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->tx_seq_ena_seq_lngth_total) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_TX_SEQ_ENA_SEQ_LNGTH_TOTAL) {
        printf("Error with tx_seq_ena_seq_lngth_total\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->tx_seq_ena_seq_lngth_part) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_TX_SEQ_ENA_SEQ_LNGTH_PART) {
        printf("Error with tx_seq_ena_seq_lngth_part\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->tx_seq_ena_cmd) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_TX_SEQ_ENA_CMD) {
        printf("Error with tx_seq_ena_cmd\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->tx_seq_dis_seq_lngth_total) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_TX_SEQ_DIS_SEQ_LNGTH_TOTAL) {
        printf("Error with tx_seq_dis_seq_lngth_total\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->tx_seq_dis_seq_lngth_part) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_TX_SEQ_DIS_SEQ_LNGTH_PART) {
        printf("Error with tx_seq_dis_seq_lngth_part\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->tx_seq_dis_cmd) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_TX_SEQ_DIS_CMD) {
        printf("Error with tx_seq_dis_cmd\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->tx_seq_spi) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_TX_SEQ_SPI) {
        printf("Error with tx_seq_spi\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rx_seq_ena_seq_lngth_total) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RX_SEQ_ENA_SEQ_LNGTH_TOTAL) {
        printf("Error with rx_seq_ena_seq_lngth_total\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rx_seq_ena_seq_lngth_part) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RX_SEQ_ENA_SEQ_LNGTH_PART) {
        printf("Error with rx_seq_ena_seq_lngth_part\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rx_seq_ena_cmd) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RX_SEQ_ENA_CMD) {
        printf("Error with rx_seq_ena_cmd\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rx_seq_dis_seq_lngth_total) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RX_SEQ_DIS_SEQ_LNGTH_TOTAL) {
        printf("Error with rx_seq_dis_seq_lngth_total\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rx_seq_dis_seq_lngth_part) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RX_SEQ_DIS_SEQ_LNGTH_PART) {
        printf("Error with rx_seq_dis_seq_lngth_part\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rx_seq_dis_cmd) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RX_SEQ_DIS_CMD) {
        printf("Error with rx_seq_dis_cmd\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rx_seq_spi) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RX_SEQ_SPI) {
        printf("Error with rx_seq_spi\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rffe_spim_cfg) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RFFE_SPIM_CFG) {
        printf("Error with rffe_spim_cfg\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rffe_spim_data_out) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RFFE_SPIM_DATA_OUT) {
        printf("Error with rffe_spim_data_out\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rffe_spim_start_transaction) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RFFE_SPIM_START_TRANSACTION) {
        printf("Error with rffe_spim_start_transaction\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->rffe_spim_data_in) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_RFFE_SPIM_DATA_IN) {
        printf("Error with rffe_spim_data_in\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_spi_cfg) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_SPI_CFG) {
        printf("Error with agc_spi_cfg\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_gain_addr) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_GAIN_ADDR) {
        printf("Error with agc_enc5_gain_addr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_gain_table) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_GAIN_TABLE) {
        printf("Error with agc_enc5_gain_table\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_gain_table_db) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_GAIN_TABLE_DB) {
        printf("Error with agc_enc5_gain_table_db\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_ready_tmr) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_READY_TMR) {
        printf("Error with agc_enc5_ready_tmr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_offs_i_addr) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_OFFS_I_ADDR) {
        printf("Error with agc_enc5_offs_i_addr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_offs_q_addr) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_OFFS_Q_ADDR) {
        printf("Error with agc_enc5_offs_q_addr\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_dc_offs) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_DC_OFFS) {
        printf("Error with agc_enc5_dc_offs\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_offs_i_bypass) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_OFFS_I_BYPASS) {
        printf("Error with agc_enc5_offs_i_bypass\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_offs_q_bypass) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_OFFS_Q_BYPASS) {
        printf("Error with agc_enc5_offs_q_bypass\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_curr_dc_offs_i) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_CURR_DC_OFFS_I) {
        printf("Error with agc_enc5_curr_dc_offs_i\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->agc_enc5_urr_dc_offs_q) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_AGC_ENC5_URR_DC_OFFS_Q) {
        printf("Error with agc_enc5_urr_dc_offs_q\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->general2_skip_fir_amp) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_GENERAL2_SKIP_FIR_AMP) {
        printf("Error with general2_skip_fir_amp\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->general2_fir_coefficients) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_GENERAL2_FIR_COEFFICIENTS) {
        printf("Error with general2_fir_coefficients\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->general2_fir_scalingfactor) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_GENERAL2_FIR_SCALINGFACTOR) {
        printf("Error with general2_fir_scalingfactor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->anti_alias_am_skip_fir) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_ANTI_ALIAS_AM_SKIP_FIR) {
        printf("Error with anti_alias_am_skip_fir\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->anti_alias_am_coeff) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_ANTI_ALIAS_AM_COEFF) {
        printf("Error with anti_alias_am_coeff\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->anti_alias_am_scalingfactor) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_ANTI_ALIAS_AM_SCALINGFACTOR) {
        printf("Error with anti_alias_am_scalingfactor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->anti_alias_fm_skip_fir) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_ANTI_ALIAS_FM_SKIP_FIR) {
        printf("Error with anti_alias_fm_skip_fir\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->anti_alias_fm_coeff) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_ANTI_ALIAS_FM_COEFF) {
        printf("Error with anti_alias_fm_coeff\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->anti_alias_fm_scalingfactor) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_ANTI_ALIAS_FM_SCALINGFACTOR) {
        printf("Error with anti_alias_fm_scalingfactor\r\n");
        return 0;
    }
    if((uint32_t)&(dbb_rffe_base->cdc_clk_div_en) != DBB_RFFE_BASE_ADDR + MXC_R_DBB_RFFE_CDC_CLK_DIV_EN) {
        printf("Error with cdc_clk_div_en\r\n");
        return 0;
    }

    return 1;
}

/*************************************************************************************************/
/*!
 *  \fn     main
 *
 *  \brief  Entry point for demo software.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
int main(void)
{

    printf("\r\n\r\n***** MAX32565 DBB/AFE register test *****\r\n");

    if(!test_afe()) {
        printf("Error with AFE registers\r\n");
    } else {
        printf("AFE registers pass!\r\n");
    }

    if(!test_dbb_ctrl()) {
        printf("Error with DBB_CTRL registers\r\n");
    } else {
        printf("DBB_CTRL registers pass!\r\n");
    }

    if(!test_dbb_tx()) {
        printf("Error with DBB_TX registers\r\n");
    } else {
        printf("DBB_TX registers pass!\r\n");
    }

    if(!test_dbb_rx()) {
        printf("Error with DBB_RX registers\r\n");
    } else {
        printf("DBB_RX registers pass!\r\n");
    }

    if(!test_dbb_rffe()) {
        printf("Error with DBB_RFFE registers\r\n");
    } else {
        printf("DBB_RFFE registers pass!\r\n");
    }

    printf("Done testing\r\n");
    while (1) {}
}
