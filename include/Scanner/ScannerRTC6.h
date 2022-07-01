#pragma once

#include "Scanner.h"

class AFX_EXT_CLASS_SCANNER CScannerRTC6 : public IScanner {
public:
	CScannerRTC6(int iBoard = 1);
	virtual ~CScannerRTC6();

	virtual eSCANNER_TYPE GetScannerType() { return SCANNER_RTC6; }

	static bool LoadDriver(LPCTSTR pszPath);
	static bool ReleaseDriver();


	virtual UINT init_rtc6_dll(void);
	virtual void free_rtc6_dll(void);
	virtual void set_rtc4_mode(void);
	virtual void set_rtc5_mode(void);
	virtual void set_rtc6_mode(void);
	virtual UINT get_rtc_mode(void);
	virtual UINT verify_checksum(const char* Name);
	virtual UINT count_cards(void);
	virtual UINT select_rtc();
	virtual UINT get_dll_version(void);
	virtual UINT transform(long& Sig1, long& Sig2, const ULONG_PTR Ptr, UINT Code);

	// Board-Index-Dependent
	virtual UINT acquire_rtc();
	virtual UINT release_rtc();
	virtual UINT get_error(void);
	virtual UINT get_last_error(void);
	virtual void reset_error(UINT Code);
	virtual UINT set_verify(UINT Verify);
	virtual UINT get_serial_number(void);
	virtual UINT get_hex_version(void);
	virtual UINT get_board_version(void);
	virtual UINT load_program_file(const char* Path);
	virtual void sync_slaves(void);
	virtual UINT get_sync_status(void);
	virtual UINT load_correction_file(const char* Name, UINT No, UINT Dim = 2);
	virtual UINT load_z_table(double A, double B, double C);
	virtual void select_cor_table(UINT HeadA, UINT HeadB);
	virtual UINT set_dsp_mode(UINT Mode);
	virtual long load_stretch_table(const char* Name, const long No, const UINT TableNo);
	virtual double get_head_para(UINT ParaNo);
	virtual double get_table_para(UINT TableNo, UINT ParaNo);
	virtual void config_list(UINT Mem1, UINT Mem2);
	virtual void get_config_list(void);
	virtual UINT save_disk(const char* Name, UINT Mode);
	virtual UINT load_disk(const char* Name, UINT Mode);
	virtual UINT get_list_space(void);
	virtual void set_start_list_pos(UINT ListNo, UINT Pos);
	virtual void set_start_list(UINT ListNo);
	virtual void set_start_list_1(void);
	virtual void set_start_list_2(void);
	virtual void set_input_pointer(UINT Pos);
	virtual UINT load_list(UINT ListNo, UINT Pos);
	virtual void load_sub(UINT Index);
	virtual void load_char(UINT Char);
	virtual void load_text_table(UINT Index);
	virtual void get_list_pointer(UINT& ListNo, UINT& Pos);
	virtual UINT get_input_pointer(void);
	virtual void execute_list_pos(UINT ListNo, UINT Pos);
	virtual void execute_at_pointer(UINT Pos);
	virtual void execute_list(UINT ListNo);
	virtual void execute_list_1(void);
	virtual void execute_list_2(void);
	virtual void get_out_pointer(UINT& ListNo, UINT& Pos);

	virtual void auto_change_pos(UINT Pos);
	virtual void start_loop(void);
	virtual void quit_loop(void);
	virtual void pause_list(void);
	virtual void restart_list(void);
	virtual void release_wait(void);
	virtual void stop_execution(void);
	virtual void auto_change(void);
	virtual void stop_list(void);
	virtual UINT get_wait_status(void);
	virtual UINT read_status(void);
	virtual void get_status(UINT& Status, UINT& Pos);

	virtual void set_extstartpos(UINT Pos);
	virtual void set_max_counts(UINT Counts);
	virtual void set_control_mode(UINT Mode);
	virtual void simulate_ext_stop(void);
	virtual void simulate_ext_start_ctrl(void);
	virtual UINT get_counts(void);
	virtual UINT get_startstop_info(void);

	virtual void copy_dst_src(UINT Dst, UINT Src, UINT Mode);
	virtual void set_char_pointer(UINT Char, UINT Pos);
	virtual void set_sub_pointer(UINT Index, UINT Pos);
	virtual void set_text_table_pointer(UINT Index, UINT Pos);
	virtual void set_char_table(UINT Index, UINT Pos);
	virtual UINT get_char_pointer(UINT Char);
	virtual UINT get_sub_pointer(UINT Index);
	virtual UINT get_text_table_pointer(UINT Index);

	virtual void time_update(void);
	virtual void set_serial_step(UINT No, UINT Step);
	virtual void set_serial(UINT No);
	virtual double get_serial(void);

	virtual void write_io_port_mask(UINT Value, UINT Mask);
	virtual void write_8bit_port(UINT Value);
	virtual UINT read_io_port(void);
	virtual UINT read_io_port_buffer(UINT Index, UINT& Value, long& XPos, long& YPos, UINT& Time);
	virtual UINT get_io_status(void);
	virtual UINT read_analog_in(void);
	virtual void write_da_x(UINT x, UINT Value);
	virtual void set_laser_off_default(UINT AnalogOut1, UINT AnalogOut2, UINT DigitalOut);
	virtual void set_port_default(UINT Port, UINT Value);
	virtual void write_io_port(UINT Value);
	virtual void write_da_1(UINT Value);
	virtual void write_da_2(UINT Value);

	virtual void disable_laser(void);
	virtual void enable_laser(void);
	virtual void laser_signal_on(void);
	virtual void laser_signal_off(void);
	virtual void set_standby(UINT HalfPeriod, UINT PulseLength);
	virtual void set_laser_pulses_ctrl(UINT HalfPeriod, UINT PulseLength);
	virtual void set_firstpulse_killer(UINT Length);
	virtual void set_qswitch_delay(UINT Delay);
	virtual void set_laser_mode(UINT Mode);
	virtual void set_laser_control(UINT Ctrl);
	virtual void set_laser_pin_out(UINT Pins);
	virtual UINT get_laser_pin_in(void);
	virtual void set_softstart_level(UINT Index, UINT Level);
	virtual void set_softstart_mode(UINT Mode, UINT Number, UINT Delay);
	virtual UINT set_auto_laser_control(UINT Ctrl, UINT Value, UINT Mode, UINT MinValue, UINT MaxValue);
	virtual UINT set_auto_laser_params(UINT Ctrl, UINT Value, UINT MinValue, UINT MaxValue);
	virtual long load_auto_laser_control(const char* Name, UINT No);
	virtual long load_position_control(const char* Name, UINT No);
	virtual void set_default_pixel(UINT PulseLength);
	virtual void get_standby(UINT& HalfPeriod, UINT& PulseLength);
	virtual void set_pulse_picking(UINT No);
	virtual void set_pulse_picking_length(UINT Length);
	virtual void config_laser_signals(UINT Config);

	virtual void set_ext_start_delay(long Delay, UINT EncoderNo);
	virtual void set_rot_center(long X, long Y);
	virtual void simulate_encoder(UINT EncoderNo);
	virtual UINT get_marking_info(void);
	virtual void set_encoder_speed_ctrl(UINT EncoderNo, double Speed, double Smooth);
	virtual void set_mcbsp_x(double ScaleX);
	virtual void set_mcbsp_y(double ScaleY);
	virtual void set_mcbsp_rot(double Resolution);
	virtual void set_mcbsp_matrix(void);
	virtual void set_mcbsp_in(UINT Mode, double Scale);
	virtual void set_fly_tracking_error(UINT TrackingErrorX, UINT TrackingErrorY);
	virtual long load_fly_2d_table(const char* Name, UINT No);
	virtual void init_fly_2d(long OffsetX, long OffsetY);
	virtual void get_fly_2d_offset(long& OffsetX, long& OffsetY);
	virtual void get_encoder(long& Encoder0, long& Encoder1);
	virtual void read_encoder(long& Encoder0_1, long& Encoder1_1, long& Encoder0_2, long& Encoder1_2);
	virtual long get_mcbsp(void);
	virtual long read_mcbsp(UINT No);

	virtual double get_time(void);
	virtual void measurement_status(UINT& Busy, UINT& Pos);
	virtual void get_waveform(UINT Channel, UINT Number, ULONG_PTR Ptr);
	virtual void bounce_supp(UINT Length);
	virtual void home_position_xyz(long XHome, long YHome, long ZHome);
	virtual void home_position(long XHome, long YHome);
	virtual void rs232_config(UINT BaudRate);
	virtual void rs232_write_data(UINT Data);
	virtual void rs232_write_text(const char* pData);
	virtual UINT rs232_read_data(void);
	virtual UINT set_mcbsp_freq(UINT Freq);
	virtual void mcbsp_init(UINT XDelay, UINT RDelay);
	virtual UINT get_overrun(void);
	virtual UINT get_master_slave(void);
	virtual void get_transform(UINT Number, ULONG_PTR Ptr1, ULONG_PTR Ptr2, ULONG_PTR Ptr, UINT Code);
	virtual void stop_trigger(void);
	//virtual void move_to(UINT Pos);
	virtual void set_enduring_wobbel(UINT CenterX, UINT CenterY, UINT CenterZ, UINT LimitHi, UINT LimitLo, double ScaleX, double ScaleY, double ScaleZ);
	virtual void set_free_variable(UINT VarNo, UINT Value);
	virtual UINT get_free_variable(UINT VarNo);
	virtual void set_mcbsp_out_ptr(UINT Number, const ULONG_PTR SignalPtr);

	virtual void set_defocus(long Shift);
	virtual void goto_xyz(long X, long Y, long Z);
	virtual void goto_xy(long X, long Y);
	virtual long get_z_distance(long X, long Y, long Z);

	virtual void set_offset_xyz(long XOffset, long YOffset, long ZOffset, UINT at_once);
	virtual void set_offset(long XOffset, long YOffset);
	virtual void set_offset(long XOffset, long YOffset, UINT at_once);
	virtual void set_matrix(double M11, double M12, double M21, double M22);
	virtual void set_matrix(double M11, double M12, double M21, double M22, UINT at_once);
	virtual void set_angle(double Angle, UINT at_once);
	virtual void set_scale(double Scale, UINT at_once);
	virtual void apply_mcbsp(UINT at_once);
	virtual UINT upload_transform(const ULONG_PTR Ptr);

	virtual void set_delay_mode(UINT VarPoly, UINT DirectMove3D, UINT EdgeLevel, UINT MinJumpDelay, UINT JumpLengthLimit);
	virtual void set_jump_speed_ctrl(double Speed);
	virtual void set_mark_speed_ctrl(double Speed);
	virtual void set_sky_writing_para(double Timelag, long LaserOnShift, UINT Nprev, UINT Npost);
	virtual void set_sky_writing_limit(double CosAngle);
	virtual void set_sky_writing_mode(UINT Mode);
	virtual long load_varpolydelay(const char* Name, UINT No);
	virtual void set_hi(double GalvoGainX, double GalvoGainY, long GalvoOffsetX, long GalvoOffsetY);
	virtual void get_hi_pos(long& X1, long& X2, long& Y1, long& Y2);
	virtual UINT auto_cal(UINT Command);
	virtual UINT get_auto_cal();
	virtual void set_sky_writing(double Timelag, long LaserOnShift);
	virtual void get_hi_data(long& X1, long& X2, long& Y1, long& Y2);

	virtual void send_user_data(UINT Axis, long Data0, long Data1, long Data2, long Data3, long Data4);
	virtual long read_user_data(UINT Axis, long& Data0, long& Data1, long& Data2, long& Data3, long& Data4);
	virtual void control_command(UINT Axis, UINT Data);
	virtual long get_value(UINT Signal);
	virtual void get_values(const ULONG_PTR SignalPtr, ULONG_PTR ResultPtr);
	virtual UINT get_head_status();
	//virtual long set_jump_mode(long Flag, UINT Length, long VA1, long VA2, long VB1, long VB2, long JA1, long JA2, long JB1, long JB2);
	//virtual long load_jump_table_offset(const char* Name, UINT No, UINT PosAck, long Offset, UINT MinDelay, UINT MaxDelay, UINT ListPos);
	//virtual UINT get_jump_table(const ULONG_PTR Ptr);
	//virtual UINT set_jump_table(const ULONG_PTR Ptr);
	//virtual long load_jump_table(const char* Name, UINT No, UINT PosAck, UINT MinDelay, UINT MaxDelay, UINT ListPos);

	virtual void stepper_init(UINT No, UINT Period, long Dir, long Pos, UINT Tol, UINT Enable, UINT WaitTime);
	virtual void stepper_enable(long Enable1, long Enable2);
	virtual void stepper_control(long Period1, long Period2);
	virtual void stepper_abs_no(UINT No, long Pos, UINT WaitTime);
	virtual void stepper_rel_no(UINT No, long dPos, UINT WaitTime);
	virtual void stepper_abs(long Pos1, long Pos2, UINT WaitTime);
	virtual void stepper_rel(long dPos1, long dPos2, UINT WaitTime);
	virtual void get_stepper_status(UINT& Status1, long& Pos1, UINT& Status2, long& Pos2);

	virtual void select_cor_table_list(UINT HeadA, UINT HeadB);

	virtual void list_nop(void);
	virtual void list_continue(void);
	virtual void long_delay(UINT Delay);
	virtual void set_end_of_list(void);
	virtual void set_wait(UINT WaitWord);
	virtual void list_jump_pos(UINT Pos);
	virtual void list_jump_rel(long Pos);
	virtual void list_repeat(void);
	virtual void list_until(UINT Number);
	virtual void set_list_jump(UINT Pos);

	virtual void set_extstartpos_list(UINT Pos);
	virtual void set_control_mode_list(UINT Mode);
	virtual void simulate_ext_start(long Delay, UINT EncoderNo);

	virtual void list_return(void);
	virtual void list_call(UINT Pos);
	virtual void list_call_abs(UINT Pos);
	virtual void sub_call(UINT Index);
	virtual void sub_call_abs(UINT Index);

	virtual void list_call_cond(UINT Mask1, UINT Mask0, UINT Pos);
	virtual void list_call_abs_cond(UINT Mask1, UINT Mask0, UINT Pos);
	virtual void sub_call_cond(UINT Mask1, UINT Mask0, UINT Index);
	virtual void sub_call_abs_cond(UINT Mask1, UINT Mask0, UINT Index);
	virtual void list_jump_pos_cond(UINT Mask1, UINT Mask0, UINT Pos);
	virtual void list_jump_rel_cond(UINT Mask1, UINT Mask0, long Pos);
	virtual void if_cond(UINT Mask1, UINT Mask0);
	virtual void if_not_cond(UINT Mask1, UINT Mask0);
	virtual void if_pin_cond(UINT Mask1, UINT Mask0);
	virtual void if_not_pin_cond(UINT Mask1, UINT Mask0);
	virtual void switch_ioport(UINT MaskBits, UINT ShiftBits);
	virtual void list_jump_cond(UINT Mask1, UINT Mask0, UINT Pos);

	virtual void select_char_set(UINT No);
	virtual void mark_text(const char* Text);
	virtual void mark_text_abs(const char* Text);
	virtual void mark_char(UINT Char);
	virtual void mark_char_abs(UINT Char);

	virtual void mark_serial(UINT Mode, UINT Digits);
	virtual void mark_serial_abs(UINT Mode, UINT Digits);
	virtual void mark_date(UINT Part, UINT Mode);
	virtual void mark_date_abs(UINT Part, UINT Mode);
	virtual void mark_time(UINT Part, UINT Mode);
	virtual void mark_time_abs(UINT Part, UINT Mode);
	virtual void time_fix_f_off(UINT FirstDay, UINT Offset);
	virtual void time_fix_f(UINT FirstDay);
	virtual void time_fix(void);

	virtual void clear_io_cond_list(UINT Mask1, UINT Mask0, UINT MaskClear);
	virtual void set_io_cond_list(UINT Mask1, UINT Mask0, UINT MaskSet);
	virtual void write_io_port_mask_list(UINT Value, UINT Mask);
	virtual void write_8bit_port_list(UINT Value);
	virtual void read_io_port_list(void);
	virtual void write_da_x_list(UINT x, UINT Value);
	virtual void write_io_port_list(UINT Value);
	virtual void write_da_1_list(UINT Value);
	virtual void write_da_2_list(UINT Value);

	virtual void laser_signal_on_list(void);
	virtual void laser_signal_off_list(void);
	virtual void para_laser_on_pulses_list(UINT Period, UINT Pulses, UINT P);
	virtual void laser_on_pulses_list(UINT Period, UINT Pulses);
	virtual void laser_on_list(UINT Period);
	virtual void set_laser_delays(long LaserOnDelay, UINT LaserOffDelay);
	virtual void set_standby_list(UINT HalfPeriod, UINT PulseLength);
	virtual void set_laser_pulses(UINT HalfPeriod, UINT PulseLength);
	virtual void set_firstpulse_killer_list(UINT Length);
	virtual void set_qswitch_delay_list(UINT Delay);
	virtual void set_laser_pin_out_list(UINT Pins);
	virtual void set_vector_control(UINT Ctrl, UINT Value);
	virtual void set_default_pixel_list(UINT PulseLength);
	virtual void set_auto_laser_params_list(UINT Ctrl, UINT Value, UINT MinValue, UINT MaxValue);
	virtual void set_pulse_picking_list(UINT No);
	virtual void set_softstart_level_list(UINT Index, UINT Level1, UINT Level2, UINT Level3);
	virtual void set_softstart_mode_list(UINT Mode, UINT Number, UINT Delay);
	virtual void set_laser_timing(UINT HalfPeriod, UINT PulseLength1, UINT PulseLength2, UINT TimeBase);

	virtual void fly_return_z(long X, long Y, long Z);
	virtual void fly_return(long X, long Y);
	virtual void set_rot_center_list(long X, long Y);
	virtual void set_ext_start_delay_list(long Delay, UINT EncoderNo);
	virtual void set_fly_x(double ScaleX);
	virtual void set_fly_y(double ScaleY);
	virtual void set_fly_z(double ScaleZ, UINT EncoderNo);
	virtual void set_fly_rot(double Resolution);
	virtual void set_fly_2d(double ScaleX, double ScaleY);
	virtual void set_fly_x_pos(double ScaleX);
	virtual void set_fly_y_pos(double ScaleY);
	virtual void set_fly_rot_pos(double Resolution);
	virtual void set_fly_limits(long Xmin, long Xmax, long Ymin, long Ymax);
	virtual void set_fly_limits_z(long Zmin, long Zmax);
	virtual void if_fly_x_overflow(long Mode);
	virtual void if_fly_y_overflow(long Mode);
	virtual void if_fly_z_overflow(long Mode);
	virtual void if_not_fly_x_overflow(long Mode);
	virtual void if_not_fly_y_overflow(long Mode);
	virtual void if_not_fly_z_overflow(long Mode);
	virtual void clear_fly_overflow(UINT Mode);
	virtual void set_mcbsp_x_list(double ScaleX);
	virtual void set_mcbsp_y_list(double ScaleY);
	virtual void set_mcbsp_rot_list(double Resolution);
	virtual void set_mcbsp_matrix_list(void);
	virtual void set_mcbsp_in_list(UINT Mode, double Scale);
	virtual void wait_for_encoder_mode(long Value, UINT EncoderNo, long Mode);
	virtual void wait_for_mcbsp(UINT Axis, long Value, long Mode);
	virtual void set_encoder_speed(UINT EncoderNo, double Speed, double Smooth);
	virtual void get_mcbsp_list(void);
	virtual void store_encoder(UINT Pos);
	virtual void wait_for_encoder_in_range(long EncXmin, long EncXmax, long EncYmin, long EncYmax);
	virtual void activate_fly_xy(double ScaleX, double ScaleY);
	virtual void activate_fly_2d(double ScaleX, double ScaleY);
	virtual void if_not_activated(void);
	virtual void park_position(UINT Mode, long X, long Y);
	virtual void park_return(UINT Mode, long X, long Y);
	virtual void wait_for_encoder(long Value, UINT EncoderNo);

	virtual void save_and_restart_timer(void);
	virtual void set_wobbel(UINT Transversal, UINT Longitudinal, double Freq);
	virtual void set_wobbel_mode(UINT Transversal, UINT Longitudinal, double Freq, long Mode);
	virtual void set_trigger(UINT Period, UINT Signal1, UINT Signal2);
	virtual void set_trigger4(UINT Period, UINT Signal1, UINT Signal2, UINT Signal3, UINT Signal4);
	virtual void set_pixel_line_3d(UINT Channel, UINT HalfPeriod, double dX, double dY, double dZ);
	virtual void set_pixel_line(UINT Channel, UINT HalfPeriod, double dX, double dY);
	virtual void set_n_pixel(UINT PulseLength, UINT AnalogOut, UINT Number);
	virtual void set_pixel(UINT PulseLength, UINT AnalogOut);
	virtual void rs232_write_text_list(const char* pData);
	virtual void set_mcbsp_out(UINT Signal1, UINT Signal2);
	virtual void camming(UINT FirstPos, UINT NPos, UINT No, UINT Ctrl, double Scale, UINT Code);
	virtual void micro_vector_abs_3d(long X, long Y, long Z, long LasOn, long LasOf);
	virtual void micro_vector_rel_3d(long dX, long dY, long dZ, long LasOn, long LasOf);
	virtual void micro_vector_abs(long X, long Y, long LasOn, long LasOf);
	virtual void micro_vector_rel(long dX, long dY, long LasOn, long LasOf);
	virtual void set_free_variable_list(UINT VarNo, UINT Value);
	virtual void control_command_list(UINT Axis, UINT Data);

	virtual void timed_mark_abs_3d(long X, long Y, long Z, double T);
	virtual void timed_mark_rel_3d(long dX, long dY, long dZ, double T);
	virtual void timed_mark_abs(long X, long Y, double T);
	virtual void timed_mark_rel(long dX, long dY, double T);

	virtual void mark_abs_3d(long X, long Y, long Z);
	virtual void mark_rel_3d(long dX, long dY, long dZ);
	virtual void mark_abs(long X, long Y);
	virtual void mark_rel(long dX, long dY);

	virtual void timed_jump_abs_3d(long X, long Y, long Z, double T);
	virtual void timed_jump_rel_3d(long dX, long dY, long dZ, double T);
	virtual void timed_jump_abs(long X, long Y, double T);
	virtual void timed_jump_rel(long dX, long dY, double T);

	virtual void jump_abs_3d(long X, long Y, long Z);
	virtual void jump_rel_3d(long dX, long dY, long dZ);
	virtual void jump_abs(long X, long Y);
	virtual void jump_rel(long dX, long dY);

	virtual void para_mark_abs_3d(long X, long Y, long Z, UINT P);
	virtual void para_mark_rel_3d(long dX, long dY, long dZ, UINT P);
	virtual void para_mark_abs(long X, long Y, UINT P);
	virtual void para_mark_rel(long dX, long dY, UINT P);

	virtual void para_jump_abs_3d(long X, long Y, long Z, UINT P);
	virtual void para_jump_rel_3d(long dX, long dY, long dZ, UINT P);
	virtual void para_jump_abs(long X, long Y, UINT P);
	virtual void para_jump_rel(long dX, long dY, UINT P);

	virtual void timed_para_mark_abs_3d(long X, long Y, long Z, UINT P, double T);
	virtual void timed_para_mark_rel_3d(long dX, long dY, long dZ, UINT P, double T);
	virtual void timed_para_jump_abs_3d(long X, long Y, long Z, UINT P, double T);
	virtual void timed_para_jump_rel_3d(long dX, long dY, long dZ, UINT P, double T);
	virtual void timed_para_mark_abs(long X, long Y, UINT P, double T);
	virtual void timed_para_mark_rel(long dX, long dY, UINT P, double T);
	virtual void timed_para_jump_abs(long X, long Y, UINT P, double T);
	virtual void timed_para_jump_rel(long dX, long dY, UINT P, double T);

	virtual void set_defocus_list(long Shift);

	virtual void timed_arc_abs(long X, long Y, double Angle, double T);
	virtual void timed_arc_rel(long dX, long dY, double Angle, double T);

	virtual void arc_abs_3d(long X, long Y, long Z, double Angle);
	virtual void arc_rel_3d(long dX, long dY, long dZ, double Angle);
	virtual void arc_abs(long X, long Y, double Angle);
	virtual void arc_rel(long dX, long dY, double Angle);
	virtual void set_ellipse(UINT A, UINT B, double Phi0, double Phi);
	virtual void mark_ellipse_abs(long X, long Y, double Alpha);
	virtual void mark_ellipse_rel(long dX, long dY, double Alpha);

	virtual void set_offset_xyz_list(long XOffset, long YOffset, long ZOffset, UINT at_once);
	virtual void set_offset_list(long XOffset, long YOffset);
	virtual void set_offset_list(long XOffset, long YOffset, UINT at_once);
	virtual void set_matrix_list(UINT Ind1, UINT Ind2, double Mij);
	virtual void set_matrix_list(UINT Ind1, UINT Ind2, double Mij, UINT at_once);
	virtual void set_angle_list(double Angle, UINT at_once);
	virtual void set_scale_list(double Scale, UINT at_once);
	virtual void apply_mcbsp_list(UINT at_once);

	virtual void set_mark_speed(double Speed);
	virtual void set_jump_speed(double Speed);
	virtual void set_sky_writing_para_list(double Timelag, long LaserOnShift, UINT Nprev, UINT Npost);
	virtual void set_sky_writing_list(double Timelag, long LaserOnShift);
	virtual void set_sky_writing_limit_list(double CosAngle);
	virtual void set_sky_writing_mode_list(UINT Mode);
	virtual void set_scanner_delays(UINT Jump, UINT Mark, UINT Polygon);
	virtual void set_jump_mode_list(long Flag);
	virtual void enduring_wobbel(void);
	virtual void set_delay_mode_list(UINT VarPoly, UINT DirectMove3D, UINT EdgeLevel, UINT MinJumpDelay, UINT JumpLengthLimit);

	virtual void stepper_enable_list(long Enable1, long Enable2);
	virtual void stepper_control_list(long Period1, long Period2);
	virtual void stepper_abs_no_list(UINT No, long Pos);
	virtual void stepper_rel_no_list(UINT No, long dPos);
	virtual void stepper_abs_list(long Pos1, long Pos2);
	virtual void stepper_rel_list(long dPos1, long dPos2);
	virtual void stepper_wait(UINT No);

// NOT Supported
	virtual void get_xyz_pos(long& x, long& y, long& z);
	virtual void get_xy_pos(long& x, long& y);
	virtual void select_list(UINT list_2);
	virtual void z_out(long z);

	virtual void set_piso_control(UINT l1, UINT l2) {}	// is not needed for RTC6

	virtual void z_out_list(long z);
	virtual void set_wobbel_xy(UINT long_wob, UINT trans_wob, double frequency);
	virtual void set_wobbel(UINT amplitude, double frequency);
	virtual void calculate_fly(UINT direction, double distance);
	virtual void set_pixel(UINT pulswidth, UINT davalue, UINT adchannel);
	virtual void set_list_mode(UINT mode);
	virtual void select_status(UINT mode);
	virtual void dsp_start(void);
	virtual UINT read_ad_x(UINT x);
	virtual UINT read_pixel_ad(UINT pos);
	virtual void control_command_RTC3(UINT channel, UINT data);
	virtual void get_waveform_RTC4(UINT channel, UINT istop, signed short *memptr);



};
