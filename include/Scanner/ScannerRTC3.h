#pragma once

#include "Scanner.h"

class AFX_EXT_CLASS_SCANNER CScannerRTC3 : public IScanner {
public:
	CScannerRTC3(int iBoard = 1);
	virtual ~CScannerRTC3();

	virtual eSCANNER_TYPE GetScannerType() { return SCANNER_RTC3; }

	static bool LoadDriver(LPCTSTR pszPath);
	static bool ReleaseDriver();

	//===============================================================================================
	// Board-Index-dependent
	//

	virtual UINT select_rtc();
	virtual long load_varpolydelay(const char* stbfilename, UINT tableno);
	virtual UINT load_program_file(const char* name);
	virtual UINT load_correction_file(const char* filename, UINT cortable, UINT nDim = 2);
	virtual UINT load_z_table(double A, double B, double C);
	virtual void list_nop(void);
	virtual void set_end_of_list(void);
	virtual void jump_abs_3d(long x, long y, long z);
	virtual void jump_abs(long x, long y);
	virtual void mark_abs_3d(long x, long y, long z);
	virtual void mark_abs(long x, long y);
	virtual void jump_rel_3d(long dx, long dy, long dz);
	virtual void jump_rel(long dx, long dy);
	virtual void mark_rel_3d(long dx, long dy, long dz);
	virtual void mark_rel(long dx, long dy);
	virtual void write_8bit_port_list(UINT value);
	virtual void write_da_1_list(UINT value);
	virtual void write_da_2_list(UINT value);
	virtual void set_matrix_list(UINT i, UINT j, double mij);
	virtual void set_defocus_list(long value);
	virtual void set_control_mode_list(UINT mode);
	virtual void set_offset_list(long xoffset, long yoffset);
	virtual void long_delay(UINT value);
	virtual void laser_on_list(UINT value);
	virtual void set_jump_speed(double speed);
	virtual void set_mark_speed(double speed);
	virtual void set_laser_delays(long ondelay, UINT offdelay);
	virtual void set_scanner_delays(UINT jumpdelay, UINT markdelay, UINT polydelay);
	virtual void set_list_jump(UINT position);
	virtual void set_input_pointer(UINT pointer);
	virtual void list_call(UINT position);
	virtual void list_return(void);
	virtual void z_out_list(long z);
	virtual void set_standby_list(UINT half_period, UINT pulse);
	virtual void timed_jump_abs(long x, long y, double time);
	virtual void timed_mark_abs(long x, long y, double time);
	virtual void timed_jump_rel(long dx, long dy, double time);
	virtual void timed_mark_rel(long dx, long dy, double time);
	virtual void set_laser_timing(UINT halfperiod, UINT pulse1, UINT pulse2, UINT timebase);
	virtual void set_wobbel_xy(UINT long_wob, UINT trans_wob, double frequency);
	virtual void set_wobbel(UINT amplitude, double frequency);
	virtual void set_fly_x(double kx);
	virtual void set_fly_y(double ky);
	virtual void set_fly_rot(double resolution);
	virtual void fly_return(long x, long y);
	virtual void calculate_fly(UINT direction, double distance);
	virtual void write_io_port_list(UINT value);
	virtual void select_cor_table_list(UINT heada, UINT headb);
	virtual void set_wait(UINT value);
	virtual void simulate_ext_start(long delay, UINT encoder);
	virtual void write_da_x_list(UINT x, UINT value);
	virtual void set_pixel_line(UINT pixelmode, UINT pixelperiod, double dx, double dy);
	virtual void set_pixel(UINT pulswidth, UINT davalue, UINT adchannel);
	virtual void set_extstartpos_list(UINT position);
	virtual void laser_signal_on_list(void);
	virtual void laser_signal_off_list(void);
	virtual void set_firstpulse_killer_list(UINT fpk);
	virtual void set_io_cond_list(UINT mask_1, UINT mask_0, UINT mask_set);
	virtual void clear_io_cond_list(UINT mask_1, UINT mask_0, UINT mask_clear);
	virtual void list_jump_cond(UINT mask_1, UINT mask_0, UINT position);
	virtual void list_call_cond(UINT mask_1, UINT mask_0, UINT position);
	virtual void save_and_restart_timer(void);
	virtual void set_ext_start_delay_list(long delay, UINT encoder);
	virtual UINT get_input_pointer(void);
	virtual void get_status(UINT& busy, UINT& position);
	virtual UINT read_status(void);
	virtual UINT get_startstop_info(void);
	virtual UINT get_marking_info(void);
	virtual void set_start_list_1(void);
	virtual void set_start_list_2(void);
	virtual void set_start_list(UINT listno);
	virtual void execute_list_1(void);
	virtual void execute_list_2(void);
	virtual void execute_list(UINT listno);
	virtual void write_8bit_port(UINT value);
	virtual void write_io_port(UINT value);
	virtual void auto_change(void);
	virtual void auto_change_pos(UINT start);
	virtual void start_loop(void);
	virtual void quit_loop(void);
	virtual void set_list_mode(UINT mode);
	virtual void stop_execution(void);
	virtual UINT read_io_port(void);
	virtual void write_da_1(UINT value);
	virtual void write_da_2(UINT value);
	virtual void set_max_counts(UINT counts);
	virtual UINT get_counts(void);
	virtual void set_matrix(double m11, double m12, double m21, double m22);
	virtual void set_offset(long xoffset, long yoffset);
	virtual void goto_xyz(long x, long y, long z);
	virtual void goto_xy(long x, long y);
	virtual UINT get_hex_version(void);
	virtual void disable_laser(void);
	virtual void enable_laser(void);
	virtual void stop_list(void);
	virtual void restart_list(void);
	virtual void get_xyz_pos(long& x, long& y, long& z);
	virtual void get_xy_pos(long& x, long& y);
	virtual void select_list(UINT list_2);
	virtual void z_out(long z);
	virtual void set_firstpulse_killer(UINT fpk);
	virtual void set_standby(UINT half_period, UINT pulse);
	virtual void laser_signal_on(void);
	virtual void laser_signal_off(void);
	virtual void set_delay_mode(UINT varpoly, UINT directmove3d, UINT edgelevel, UINT minjumpdelay, UINT jumplengthlimit);
	virtual void set_piso_control(UINT l1, UINT l2);
	virtual void select_status(UINT mode);
	virtual void get_encoder(long& zx, long& zy);
	virtual void select_cor_table(UINT HeadA, UINT HeadB);
	virtual void execute_at_pointer(UINT position);
	virtual UINT get_head_status();
	virtual void simulate_encoder(UINT channel);
	virtual void set_hi(double galvogainx, double galvogainy, long galvooffsetx, long galvooffsety);
	virtual void release_wait(void);
	virtual UINT get_wait_status(void);
	virtual void set_control_mode(UINT mode);
	virtual void set_laser_mode(UINT mode);
	virtual void set_ext_start_delay(long delay, UINT encoder);
	virtual void home_position(long xhome, long yhome);
	virtual void set_rot_center(long center_x, long center_y);
	virtual void dsp_start(void);
	virtual void write_da_x(UINT x, UINT value);
	virtual UINT read_ad_x(UINT x);
	virtual UINT read_pixel_ad(UINT pos);
	virtual long get_z_distance(long x, long y, long z);
	virtual UINT get_io_status(void);
	virtual double get_time(void);
	virtual void set_defocus(long value);
	virtual void set_softstart_mode(UINT mode, UINT number, UINT resetdelay);
	virtual void set_softstart_level(UINT index, UINT level);
	virtual UINT get_serial_number(void);
	virtual UINT get_board_version(void);
	virtual UINT auto_cal(UINT command);
	virtual UINT get_list_space(void);
	virtual long get_value(UINT signal);
	// RTC3 only
	virtual void control_command_RTC3(UINT channel, UINT data);
	// RTC4
	virtual void get_waveform_RTC4(UINT channel, UINT istop, signed short *memptr);
	virtual void measurement_status(UINT& busy, UINT& position);
	virtual void set_trigger(UINT sampleperiod, UINT signal1, UINT signal2);
	virtual void arc_rel(long dx, long dy, double angle);
	virtual void arc_abs(long x, long y, double angle);
	virtual void control_command(UINT axis, UINT data);
	//virtual long get_serial_number_32(void);
	//virtual void set_io_bit(UINT mask1);
	//virtual void clear_io_bit(UINT mask0);
	//virtual void move_to(UINT position);

	//===============================================================================================
	// Board-Index-independent
	//
	//virtual long getmemory(UINT adr);
	virtual UINT get_dll_version(void);
	//virtual void aut_change(void);

	//virtual long load_cor(const char* filename);
	//virtual long load_pro(const char* filename);
	//virtual void get_hi_data(UINT& x1, UINT& x2, UINT& y1, UINT& y2);
	//virtual long teachin(const char* filename, long xin, long yin, long zin, double ll0, long& xout, long& yout, long& zout);

	virtual UINT count_cards(void);

	//// RTC3 only
	//virtual void write_16bit_port(UINT value);
	//virtual void set_bases(UINT base);
	//virtual void set_ext2_mode(UINT mode);
	//virtual void field_jump(long x, long y);
	//virtual void home_jump(long x, long y);
	//virtual void set_mode(UINT mode);
	//virtual void set_delays(UINT sp, UINT jd, UINT md, UINT pd, UINT loffd, UINT lond, UINT t1, UINT t2, UINT t3);
	//virtual UINT get_rtc2_mode(void);
	//virtual void set_base(UINT value);
	//virtual void set_co2_standby(UINT half_period, UINT pulse);
	//virtual void set_co2_standby_list(UINT half_period, UINT pulse);
	//virtual void write_da(UINT value);
	//virtual void pola_abs(long x, long y);
	//virtual void polb_abs(long x, long y);
	//virtual void polc_abs(long x, long y);
	//virtual void write_da_list(UINT value);
	//virtual void laser_on(UINT value);
	//virtual void set_speed(double jumpspeed, double markspeed);
	//virtual void set_gain(double xgain, double ygain, long xoffset, long yoffset);
	//virtual UINT get_version(void);
	//virtual void set_yag_parameter(UINT parameter);
	//virtual void set_encoder_values(double kx, double ky, long zx, long zy);
	//virtual UINT get_rtc2_version(void);

	//virtual void set_list_pointer_RTC3(UINT list, UINT pointer);
	//virtual void set_line_RTC3(long dx, long dy, UINT n, UINT period, UINT& ptr);

	//// RTC4
	//virtual void drilling(long pulsewidth, long relencoderdelay);
	//virtual void regulation(void);
	//virtual void flyline(long encoderdelay);
	//virtual void set_duty_cycle_table(UINT index, UINT dutycycle);

};
