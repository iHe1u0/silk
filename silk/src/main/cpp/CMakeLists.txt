cmake_minimum_required(VERSION 3.22.1)

project(silk)

include_directories(
        interface
        src
)

set(silk_c_SOURCES
        src/SKP_Silk_A2NLSF.c
        src/SKP_Silk_ana_filt_bank_1.c
        src/SKP_Silk_apply_sine_window.c
        src/SKP_Silk_array_maxabs.c
        src/SKP_Silk_autocorr.c
        src/SKP_Silk_biquad.c
        src/SKP_Silk_biquad_alt.c
        src/SKP_Silk_burg_modified.c
        src/SKP_Silk_bwexpander.c
        src/SKP_Silk_bwexpander_32.c
        src/SKP_Silk_CNG.c
        src/SKP_Silk_code_signs.c
        src/SKP_Silk_control_audio_bandwidth.c
        src/SKP_Silk_control_codec_FIX.c
        src/SKP_Silk_corrMatrix_FIX.c
        src/SKP_Silk_create_init_destroy.c
        src/SKP_Silk_dec_API.c
        src/SKP_Silk_decode_core.c
        src/SKP_Silk_decode_frame.c
        src/SKP_Silk_decode_parameters.c
        src/SKP_Silk_decode_pitch.c
        src/SKP_Silk_decode_pulses.c
        src/SKP_Silk_decoder_set_fs.c
        src/SKP_Silk_detect_SWB_input.c
        src/SKP_Silk_div_oabi.c
        src/SKP_Silk_enc_API.c
        src/SKP_Silk_encode_frame_FIX.c
        src/SKP_Silk_encode_parameters.c
        src/SKP_Silk_encode_pulses.c
        src/SKP_Silk_find_LPC_FIX.c
        src/SKP_Silk_find_LTP_FIX.c
        src/SKP_Silk_find_pitch_lags_FIX.c
        src/SKP_Silk_find_pred_coefs_FIX.c
        src/SKP_Silk_gain_quant.c
        src/SKP_Silk_HP_variable_cutoff_FIX.c
        src/SKP_Silk_init_encoder_FIX.c
        src/SKP_Silk_inner_prod_aligned.c
        src/SKP_Silk_interpolate.c
        src/SKP_Silk_k2a.c
        src/SKP_Silk_k2a_Q16.c
        src/SKP_Silk_LBRR_reset.c
        src/SKP_Silk_lin2log.c
        src/SKP_Silk_log2lin.c
        src/SKP_Silk_LP_variable_cutoff.c
        src/SKP_Silk_LPC_inv_pred_gain.c
        src/SKP_Silk_LPC_synthesis_filter.c
        src/SKP_Silk_LPC_synthesis_order16.c
        src/SKP_Silk_LSF_cos_table.c
        src/SKP_Silk_LTP_analysis_filter_FIX.c
        src/SKP_Silk_LTP_scale_ctrl_FIX.c
        src/SKP_Silk_MA.c
        src/SKP_Silk_NLSF2A.c
        src/SKP_Silk_NLSF2A_stable.c
        src/SKP_Silk_NLSF_MSVQ_decode.c
        src/SKP_Silk_NLSF_MSVQ_encode_FIX.c
        src/SKP_Silk_NLSF_stabilize.c
        src/SKP_Silk_NLSF_VQ_rate_distortion_FIX.c
        src/SKP_Silk_NLSF_VQ_sum_error_FIX.c
        src/SKP_Silk_NLSF_VQ_weights_laroia.c
        src/SKP_Silk_noise_shape_analysis_FIX.c
        src/SKP_Silk_NSQ.c
        src/SKP_Silk_NSQ_del_dec.c
        src/SKP_Silk_pitch_analysis_core.c
        src/SKP_Silk_pitch_est_tables.c
        src/SKP_Silk_PLC.c
        src/SKP_Silk_prefilter_FIX.c
        src/SKP_Silk_process_gains_FIX.c
        src/SKP_Silk_process_NLSFs_FIX.c
        src/SKP_Silk_quant_LTP_gains_FIX.c
        src/SKP_Silk_range_coder.c
        src/SKP_Silk_regularize_correlations_FIX.c
        src/SKP_Silk_resampler.c
        src/SKP_Silk_resampler_down2.c
        src/SKP_Silk_resampler_down2_3.c
        src/SKP_Silk_resampler_down3.c
        src/SKP_Silk_resampler_private_AR2.c
        src/SKP_Silk_resampler_private_ARMA4.c
        src/SKP_Silk_resampler_private_copy.c
        src/SKP_Silk_resampler_private_down4.c
        src/SKP_Silk_resampler_private_down_FIR.c
        src/SKP_Silk_resampler_private_IIR_FIR.c
        src/SKP_Silk_resampler_private_up2_HQ.c
        src/SKP_Silk_resampler_private_up4.c
        src/SKP_Silk_resampler_rom.c
        src/SKP_Silk_resampler_up2.c
        src/SKP_Silk_residual_energy16_FIX.c
        src/SKP_Silk_residual_energy_FIX.c
        src/SKP_Silk_scale_copy_vector16.c
        src/SKP_Silk_scale_vector.c
        src/SKP_Silk_schur.c
        src/SKP_Silk_schur64.c
        src/SKP_Silk_shell_coder.c
        src/SKP_Silk_sigm_Q15.c
        src/SKP_Silk_solve_LS_FIX.c
        src/SKP_Silk_sort.c
        src/SKP_Silk_sum_sqr_shift.c
        src/SKP_Silk_tables_gain.c
        src/SKP_Silk_tables_LTP.c
        src/SKP_Silk_tables_NLSF_CB0_10.c
        src/SKP_Silk_tables_NLSF_CB0_16.c
        src/SKP_Silk_tables_NLSF_CB1_10.c
        src/SKP_Silk_tables_NLSF_CB1_16.c
        src/SKP_Silk_tables_other.c
        src/SKP_Silk_tables_pitch_lag.c
        src/SKP_Silk_tables_pulses_per_block.c
        src/SKP_Silk_tables_sign.c
        src/SKP_Silk_tables_type_offset.c
        src/SKP_Silk_VAD.c
        src/SKP_Silk_VQ_nearest_neighbor_FIX.c
        src/SKP_Silk_warped_autocorrelation_FIX.c
        )

set(silk_arm_SOURCES
        src/SKP_Silk_A2NLSF_arm.S
        src/SKP_Silk_allpass_int_arm.S
        src/SKP_Silk_ana_filt_bank_1_arm.S
        src/SKP_Silk_array_maxabs_arm.S
        src/SKP_Silk_clz_arm.S
        src/SKP_Silk_decode_core_arm.S
        src/SKP_Silk_inner_prod_aligned_arm.S
        src/SKP_Silk_lin2log_arm.S
        src/SKP_Silk_MA_arm.S
        src/SKP_Silk_NLSF_VQ_sum_error_FIX_arm.S
        src/SKP_Silk_prefilter_FIX_arm.S
        src/SKP_Silk_resampler_down2_arm.S
        src/SKP_Silk_resampler_private_AR2_arm.S
        src/SKP_Silk_resampler_private_ARMA4_arm.S
        src/SKP_Silk_resampler_private_down_FIR_arm.S
        src/SKP_Silk_resampler_private_IIR_FIR_arm.S
        src/SKP_Silk_resampler_private_up2_HQ_arm.S
        src/SKP_Silk_resampler_rom_arm.S
        src/SKP_Silk_resampler_up2_arm.S
        src/SKP_Silk_schur64_arm.S
        src/SKP_Silk_sigm_Q15_arm.S
        src/SKP_Silk_sum_sqr_shift_arm.S
        src/SKP_Silk_warped_autocorrelation_FIX_arm.S
        )

set(silk_private_HEADERS
        src/SKP_Silk_AsmHelper.h
        src/SKP_Silk_AsmPreproc.h
        src/SKP_Silk_common_pitch_est_defines.h
        src/SKP_Silk_define.h
        src/SKP_Silk_Inlines.h
        src/SKP_Silk_macros.h
        src/SKP_Silk_macros_arm.h
        src/SKP_Silk_main.h
        src/SKP_Silk_main_FIX.h
        src/SKP_Silk_pitch_est_defines.h
        src/SKP_Silk_PLC.h
        src/SKP_Silk_resampler_private.h
        src/SKP_Silk_resampler_rom.h
        src/SKP_Silk_resampler_structs.h
        src/SKP_Silk_setup_complexity.h
        src/SKP_Silk_SigProc_FIX.h
        src/SKP_Silk_structs.h
        src/SKP_Silk_structs_FIX.h
        src/SKP_Silk_tables.h
        src/SKP_Silk_tables_NLSF_CB0_10.h
        src/SKP_Silk_tables_NLSF_CB0_16.h
        src/SKP_Silk_tables_NLSF_CB1_10.h
        src/SKP_Silk_tables_NLSF_CB1_16.h
        src/SKP_Silk_tuning_parameters.h
        )

set(silk_public_HEADERS
        interface/SKP_Silk_control.h
        interface/SKP_Silk_errors.h
        interface/SKP_Silk_SDK_API.h
        interface/SKP_Silk_typedef.h
        )

set(silk_SOURCES
        ${silk_c_SOURCES}
        ${silk_private_HEADERS}
        ${silk_public_HEADERS}
        )

if (USE_ARM_ASSEMBLY)
    set(silk_SOURCES
            ${silk_SOURCES}
            ${silk_arm_SOURCES}
            )
    enable_language(ASM)
else ()
    add_definitions(-DNO_ASM)
endif ()

if (NOT WIN32 AND NOT APPLE)
    add_compile_options(-Wall -fPIC)
endif ()

if (NOT WIN32)
    add_compile_options(-Wno-shift-negative-value)
endif ()

add_library(silk-v3 STATIC ${silk_SOURCES})

add_library(silk SHARED jni_init.cpp silk.cpp coder.cpp)

find_library(log-lib log)

target_link_libraries(silk
        silk-v3
        ${log-lib})