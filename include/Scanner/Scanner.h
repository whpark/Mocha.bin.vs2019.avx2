#pragma once

#include "AFX_EXT_SCANNER.h"

//#ifndef NULL
//#	define	NULL 0
//#endif

enum eSCANNER_TYPE : int { SCANNER_NONE, SCANNER_DUMMY, SCANNER_RTC3, SCANNER_RTC4, SCANNER_RTC5, SCANNER_RTC6, SCANNER_GB501, };

class AFX_EXT_CLASS_SCANNER IScanner {
protected:
	UINT m_iBoard;
	UINT m_iHead = 1;

public:
	enum eTRIGGER_SIGNAL {

		// RTC4/RTC5
		SIGNAL_LASER_ON,
		SIGNAL_STATUS_AX,
		SIGNAL_STATUS_AY,
		SIGNAL_reserved,
		SIGNAL_STATUS_BX,
		SIGNAL_STATUS_BY,
		SIGNAL_reserved2,
		SIGNAL_SAMPLE_X,
		SIGNAL_SAMPLE_Y,
		SIGNAL_SAMPLE_Z,
		SIGNAL_SAMPLE_AX_CORR,
		SIGNAL_SAMPLE_AY_CORR,
		SIGNAL_SAMPLE_AZ_CORR,
		SIGNAL_SAMPLE_BX_CORR,
		SIGNAL_SAMPLE_BY_CORR,
		SIGNAL_SAMPLE_BZ_CORR,

		// RTC5
		SIGNAL_STATUS_AX_AND_LASER_ON,		// STATUS_AX for laser signal on, -524,288 for laser signal off
		SIGNAL_STATUS_AY_AND_LASER_ON,		// STATUS_AY for laser signal on, -524,288 for laser signal off
		SIGNAL_STATUS_BX_AND_LASER_ON,		// STATUS_BX for laser signal on, -524,288 for laser signal off
		SIGNAL_STATUS_BY_AND_LASER_ON,		// STATUS_BY for laser signal on, -524,288 for laser signal off
		SIGNAL_SAMPLE_AX_OUT,				// effective X-axis output value for the primary scan head connector; if applicable incl. any scanner offset and gain compensation, see comments in manual.
		SIGNAL_SAMPLE_AY_OUT,
		SIGNAL_SAMPLE_BX_OUT,
		SIGNAL_SAMPLE_BY_OUT,
		SIGANL_LASER_CONTROL_PARAMETER,		// Laser control parameter of automatic laser control


	};

public:
	IScanner(int iBoard = 0) : m_iBoard(iBoard) { }
	virtual ~IScanner() {}

	virtual eSCANNER_TYPE GetScannerType() = NULL;//{ return SCANNER_NONE; }

	uintXX_t GetBoard() const { return m_iBoard; }
	uintXX_t GetHead() const { return m_iHead; }

	//===============================================================================================
	// Board-Index-dependent
	//
	virtual UINT select_board(int iBoard /* 1 base */) { int iBoardOld = m_iBoard; m_iBoard = iBoard; return iBoardOld; }
	virtual long load_varpolydelay(const char* stbfilename, UINT tableno) = NULL;
	virtual UINT load_program_file(const char* name) = NULL;
	virtual UINT load_correction_file(const char* filename, UINT cortable, UINT nDim = 2) = NULL;
	virtual UINT load_z_table(double A, double B, double C) = NULL;
	virtual void list_nop(void) = NULL;
	virtual void set_end_of_list(void) = NULL;
	virtual void jump_abs_3d(long x, long y, long z) = NULL;
	virtual void jump_abs(long x, long y) = NULL;
	virtual void mark_abs_3d(long x, long y, long z) = NULL;
	virtual void mark_abs(long x, long y) = NULL;
	virtual void jump_rel_3d(long dx, long dy, long dz) = NULL;
	virtual void jump_rel(long dx, long dy) = NULL;
	virtual void mark_rel_3d(long dx, long dy, long dz) = NULL;
	virtual void mark_rel(long dx, long dy) = NULL;
	virtual void write_8bit_port_list(UINT value) = NULL;
	virtual void write_da_1_list(UINT value) = NULL;
	virtual void write_da_2_list(UINT value) = NULL;
	virtual void set_matrix_list(UINT i, UINT j, double mij) = NULL;
	virtual void set_defocus_list(long value) = NULL;
	virtual void set_control_mode_list(UINT mode) = NULL;
	virtual void set_offset_list(long xoffset, long yoffset) = NULL;
	virtual void long_delay(UINT value) = NULL;
	virtual void laser_on_list(UINT value) = NULL;
	virtual void set_jump_speed(double speed) = NULL;
	virtual void set_mark_speed(double speed) = NULL;
	virtual void set_laser_delays(long ondelay, UINT offdelay) = NULL;
	virtual void set_scanner_delays(UINT jumpdelay, UINT markdelay, UINT polydelay) = NULL;
	virtual void set_list_jump(UINT position) = NULL;
	virtual void set_input_pointer(UINT pointer) = NULL;
	virtual void list_call(UINT position) = NULL;
	virtual void list_return(void) = NULL;
	virtual void z_out_list(long z) = NULL;
	virtual void set_standby_list(UINT half_period, UINT pulse) = NULL;
	virtual void timed_jump_abs(long x, long y, double time) = NULL;
	virtual void timed_mark_abs(long x, long y, double time) = NULL;
	virtual void timed_jump_rel(long dx, long dy, double time) = NULL;
	virtual void timed_mark_rel(long dx, long dy, double time) = NULL;
	virtual void set_laser_timing(UINT halfperiod, UINT pulse1, UINT pulse2, UINT timebase) = NULL;
	virtual void set_wobbel_xy(UINT long_wob, UINT trans_wob, double frequency) = NULL;
	virtual void set_wobbel(UINT amplitude, double frequency) = NULL;
	virtual void set_fly_x(double kx) = NULL;
	virtual void set_fly_y(double ky) = NULL;
	virtual void set_fly_rot(double resolution) = NULL;
	virtual void fly_return(long x, long y) = NULL;
	virtual void calculate_fly(UINT direction, double distance) = NULL;
	virtual void write_io_port_list(UINT value) = NULL;
	virtual void select_cor_table_list(UINT heada, UINT headb) = NULL;
	virtual void set_wait(UINT value) = NULL;
	virtual void simulate_ext_start(long delay, UINT encoder) = NULL;
	virtual void write_da_x_list(UINT x, UINT value) = NULL;
	virtual void set_pixel_line(UINT pixelmode, UINT pixelperiod, double dx, double dy) = NULL;
	virtual void set_pixel(UINT pulswidth, UINT davalue, UINT adchannel) = NULL;
	virtual void set_extstartpos_list(UINT position) = NULL;
	virtual void laser_signal_on_list(void) = NULL;
	virtual void laser_signal_off_list(void) = NULL;
	virtual void set_firstpulse_killer_list(UINT fpk) = NULL;
	virtual void set_io_cond_list(UINT mask_1, UINT mask_0, UINT mask_set) = NULL;
	virtual void clear_io_cond_list(UINT mask_1, UINT mask_0, UINT mask_clear) = NULL;
	virtual void list_jump_cond(UINT mask_1, UINT mask_0, UINT position) = NULL;
	virtual void list_call_cond(UINT mask_1, UINT mask_0, UINT position) = NULL;
	virtual void save_and_restart_timer(void) = NULL;
	virtual void set_ext_start_delay_list(long delay, UINT encoder) = NULL;
	virtual UINT get_input_pointer(void) = NULL;
	virtual void get_status(UINT& busy, UINT& position) = NULL;
	virtual UINT read_status(void) = NULL;
	virtual UINT get_startstop_info(void) = NULL;
	virtual UINT get_marking_info(void) = NULL;
	virtual void set_start_list_1(void) = NULL;
	virtual void set_start_list_2(void) = NULL;
	virtual void set_start_list(UINT listno) = NULL;
	virtual void execute_list_1(void) = NULL;
	virtual void execute_list_2(void) = NULL;
	virtual void execute_list(UINT listno) = NULL;
	virtual void write_8bit_port(UINT value) = NULL;
	virtual void write_io_port(UINT value) = NULL;
	virtual void auto_change(void) = NULL;
	virtual void auto_change_pos(UINT start) = NULL;
	virtual void start_loop(void) = NULL;
	virtual void quit_loop(void) = NULL;
	virtual void set_list_mode(UINT mode) = NULL;
	virtual void stop_execution(void) = NULL;
	virtual UINT read_io_port(void) = NULL;
	virtual void write_da_1(UINT value) = NULL;
	virtual void write_da_2(UINT value) = NULL;
	virtual void set_max_counts(UINT counts) = NULL;
	virtual UINT get_counts(void) = NULL;
	virtual void set_matrix(double m11, double m12, double m21, double m22) = NULL;
	virtual void set_offset(long xoffset, long yoffset) = NULL;
	virtual void goto_xyz(long x, long y, long z) = NULL;
	virtual void goto_xy(long x, long y) = NULL;
	virtual UINT get_hex_version(void) = NULL;
	virtual void disable_laser(void) = NULL;
	virtual void enable_laser(void) = NULL;
	virtual void stop_list(void) = NULL;
	virtual void restart_list(void) = NULL;
	virtual void get_xyz_pos(long& x, long& y, long& z) = NULL;
	virtual void get_xy_pos(long& x, long& y) = NULL;
	virtual void select_list(UINT list_2) = NULL;
	virtual void z_out(long z) = NULL;
	virtual void set_firstpulse_killer(UINT fpk) = NULL;
	virtual void set_standby(UINT half_period, UINT pulse) = NULL;
	virtual void laser_signal_on(void) = NULL;
	virtual void laser_signal_off(void) = NULL;
	virtual void set_delay_mode(UINT varpoly, UINT directmove3d, UINT edgelevel, UINT minjumpdelay, UINT jumplengthlimit) = NULL;
	virtual void set_piso_control(UINT l1, UINT l2) = NULL;
	virtual void select_status(UINT mode) = NULL;
	virtual void get_encoder(long& zx, long& zy) = NULL;
	virtual void select_cor_table(UINT HeadA, UINT HeadB) = NULL;
	virtual void execute_at_pointer(UINT position) = NULL;
	virtual UINT get_head_status() = NULL;
	virtual void simulate_encoder(UINT channel) = NULL;
	virtual void set_hi(double galvogainx, double galvogainy, long galvooffsetx, long galvooffsety) = NULL;
	virtual void release_wait(void) = NULL;
	virtual UINT get_wait_status(void) = NULL;
	virtual void set_control_mode(UINT mode) = NULL;
	virtual void set_laser_mode(UINT mode) = NULL;
	virtual void set_ext_start_delay(long delay, UINT encoder) = NULL;
	virtual void home_position(long xhome, long yhome) = NULL;
	virtual void set_rot_center(long center_x, long center_y) = NULL;
	virtual void dsp_start(void) = NULL;
	virtual void write_da_x(UINT x, UINT value) = NULL;
	virtual UINT read_ad_x(UINT x) = NULL;
	virtual UINT read_pixel_ad(UINT pos) = NULL;
	virtual long get_z_distance(long x, long y, long z) = NULL;
	virtual UINT get_io_status(void) = NULL;
	virtual double get_time(void) = NULL;
	virtual void set_defocus(long value) = NULL;
	virtual void set_softstart_mode(UINT mode, UINT number, UINT resetdelay) = NULL;
	virtual void set_softstart_level(UINT index, UINT level) = NULL;
	virtual UINT get_serial_number(void) = NULL;
	virtual UINT get_board_version(void) = NULL;	// get_rtc_version
	virtual UINT auto_cal(UINT command) = NULL;
	virtual UINT get_list_space(void) = NULL;
	virtual long get_value(UINT signal) = NULL;
	// RTC3 only
	//virtual void set_bases(UINT base) = NULL;
	virtual void control_command_RTC3(UINT channel, UINT data) = NULL;
	// RTC4
	virtual void get_waveform_RTC4(UINT channel, UINT istop, signed short *memptr) = NULL;
	virtual void measurement_status(UINT& busy, UINT& position) = NULL;
	virtual void set_trigger(UINT sampleperiod, UINT signal1, UINT signal2) = NULL;
	virtual void arc_rel(long dx, long dy, double angle) = NULL;
	virtual void arc_abs(long x, long y, double angle) = NULL;
	virtual void control_command(UINT axis, UINT data) = NULL;
	//virtual long get_serial_number_32(void) = NULL;
	//virtual void set_io_bit(UINT mask1) = NULL;
	//virtual void clear_io_bit(UINT mask0) = NULL;
	//virtual void move_to(UINT position) = NULL;

	//===============================================================================================
	// Board-Index-independent
	//
	//virtual long getmemory(UINT adr) = NULL;
	virtual UINT get_dll_version(void) = NULL;
	//virtual void aut_change(void) = NULL;

	//virtual long load_cor(const char* filename) = NULL;
	//virtual long load_pro(const char* filename) = NULL;
	//virtual void get_hi_data(UINT& x1, UINT& x2, UINT& y1, UINT& y2) = NULL;
	//virtual long teachin(const char* filename, long xin, long yin, long zin, double ll0, long& xout, long& yout, long& zout) = NULL;

	virtual UINT count_cards(void) = NULL;

	//// RTC3 only
	//virtual void write_16bit_port(UINT value) = NULL;
	//virtual void set_ext2_mode(UINT mode) = NULL;
	//virtual void field_jump(long x, long y) = NULL;
	//virtual void home_jump(long x, long y) = NULL;
	//virtual void set_mode(UINT mode) = NULL;
	//virtual void set_delays(UINT sp, UINT jd, UINT md, UINT pd, UINT loffd, UINT lond, UINT t1, UINT t2, UINT t3) = NULL;
	//virtual UINT get_rtc2_mode(void) = NULL;
	//virtual void set_base(UINT value) = NULL;
	//virtual void set_co2_standby(UINT half_period, UINT pulse) = NULL;
	//virtual void set_co2_standby_list(UINT half_period, UINT pulse) = NULL;
	//virtual void write_da(UINT value) = NULL;
	//virtual void pola_abs(long x, long y) = NULL;
	//virtual void polb_abs(long x, long y) = NULL;
	//virtual void polc_abs(long x, long y) = NULL;
	//virtual void write_da_list(UINT value) = NULL;
	//virtual void laser_on(UINT value) = NULL;
	//virtual void set_speed(double jumpspeed, double markspeed) = NULL;
	//virtual void set_gain(double xgain, double ygain, long xoffset, long yoffset) = NULL;
	//virtual UINT get_version(void) = NULL;
	//virtual void set_yag_parameter(UINT parameter) = NULL;
	//virtual void set_encoder_values(double kx, double ky, long zx, long zy) = NULL;
	//virtual UINT get_rtc2_version(void) = NULL;

	//virtual void set_list_pointer_RTC3(UINT list, UINT pointer) = NULL;
	//virtual void set_line_RTC3(long dx, long dy, UINT n, UINT period, UINT& ptr) = NULL;

	//// RTC4
	//virtual void drilling(long pulsewidth, long relencoderdelay) = NULL;
	//virtual void regulation(void) = NULL;
	//virtual void flyline(long encoderdelay) = NULL;
	//virtual void set_duty_cycle_table(UINT index, UINT dutycycle) = NULL;

};



//---------------------------------------------------------------------------
// Dummy
class AFX_EXT_CLASS_SCANNER CScannerDummy : public IScanner {
public:
	CScannerDummy(int iBoard = 0) : IScanner(iBoard) { }
	virtual ~CScannerDummy() {}

	virtual eSCANNER_TYPE GetScannerType() { return SCANNER_DUMMY; }

	//===============================================================================================
	// Board-Index-dependent
	//
	virtual UINT select_board(int iBoard) { __super::select_board(iBoard); return 0; };
	virtual long load_varpolydelay(const char* stbfilename, UINT tableno) { return 0; };
	virtual UINT load_program_file(const char* name) { return 0; };
	virtual UINT load_correction_file(const char* filename, UINT cortable, UINT nDim = 2) { return 0; };
	virtual UINT load_z_table(double A, double B, double C) { return 0; };
	virtual void list_nop(void) { };
	virtual void set_end_of_list(void) { };
	virtual void jump_abs_3d(long x, long y, long z) { };
	virtual void jump_abs(long x, long y) { };
	virtual void mark_abs_3d(long x, long y, long z) { };
	virtual void mark_abs(long x, long y) { };
	virtual void jump_rel_3d(long dx, long dy, long dz) { };
	virtual void jump_rel(long dx, long dy) { };
	virtual void mark_rel_3d(long dx, long dy, long dz) { };
	virtual void mark_rel(long dx, long dy) { };
	virtual void write_8bit_port_list(UINT value) { };
	virtual void write_da_1_list(UINT value) { };
	virtual void write_da_2_list(UINT value) { };
	virtual void set_matrix_list(UINT i, UINT j, double mij) { };
	virtual void set_defocus_list(long value) { };
	virtual void set_control_mode_list(UINT mode) { };
	virtual void set_offset_list(long xoffset, long yoffset) { };
	virtual void long_delay(UINT value) { };
	virtual void laser_on_list(UINT value) { };
	virtual void set_jump_speed(double speed) { };
	virtual void set_mark_speed(double speed) { };
	virtual void set_laser_delays(long ondelay, UINT offdelay) { };
	virtual void set_scanner_delays(UINT jumpdelay, UINT markdelay, UINT polydelay) { };
	virtual void set_list_jump(UINT position) { };
	virtual void set_input_pointer(UINT pointer) { };
	virtual void list_call(UINT position) { };
	virtual void list_return(void) { };
	virtual void z_out_list(long z) { };
	virtual void set_standby_list(UINT half_period, UINT pulse) { };
	virtual void timed_jump_abs(long x, long y, double time) { };
	virtual void timed_mark_abs(long x, long y, double time) { };
	virtual void timed_jump_rel(long dx, long dy, double time) { };
	virtual void timed_mark_rel(long dx, long dy, double time) { };
	virtual void set_laser_timing(UINT halfperiod, UINT pulse1, UINT pulse2, UINT timebase) { };
	virtual void set_wobbel_xy(UINT long_wob, UINT trans_wob, double frequency) { };
	virtual void set_wobbel(UINT amplitude, double frequency) { };
	virtual void set_fly_x(double kx) { };
	virtual void set_fly_y(double ky) { };
	virtual void set_fly_rot(double resolution) { };
	virtual void fly_return(long x, long y) { };
	virtual void calculate_fly(UINT direction, double distance) { };
	virtual void write_io_port_list(UINT value) { };
	virtual void select_cor_table_list(UINT heada, UINT headb) { };
	virtual void set_wait(UINT value) { };
	virtual void simulate_ext_start(long delay, UINT encoder) { };
	virtual void write_da_x_list(UINT x, UINT value) { };
	virtual void set_pixel_line(UINT pixelmode, UINT pixelperiod, double dx, double dy) { };
	virtual void set_pixel(UINT pulswidth, UINT davalue, UINT adchannel) { };
	virtual void set_extstartpos_list(UINT position) { };
	virtual void laser_signal_on_list(void) { };
	virtual void laser_signal_off_list(void) { };
	virtual void set_firstpulse_killer_list(UINT fpk) { };
	virtual void set_io_cond_list(UINT mask_1, UINT mask_0, UINT mask_set) { };
	virtual void clear_io_cond_list(UINT mask_1, UINT mask_0, UINT mask_clear) { };
	virtual void list_jump_cond(UINT mask_1, UINT mask_0, UINT position) { };
	virtual void list_call_cond(UINT mask_1, UINT mask_0, UINT position) { };
	virtual void save_and_restart_timer(void) { };
	virtual void set_ext_start_delay_list(long delay, UINT encoder) { };
	virtual UINT get_input_pointer(void) { return 0; };
	virtual void get_status(UINT& busy, UINT& position) { };
	virtual UINT read_status(void) { return 0; };
	virtual UINT get_startstop_info(void) { return 0; };
	virtual UINT get_marking_info(void) { return 0; };
	virtual void set_start_list_1(void) { };
	virtual void set_start_list_2(void) { };
	virtual void set_start_list(UINT listno) { };
	virtual void execute_list_1(void) { };
	virtual void execute_list_2(void) { };
	virtual void execute_list(UINT listno) { };
	virtual void write_8bit_port(UINT value) { };
	virtual void write_io_port(UINT value) { };
	virtual void auto_change(void) { };
	virtual void auto_change_pos(UINT start) { };
	virtual void start_loop(void) { };
	virtual void quit_loop(void) { };
	virtual void set_list_mode(UINT mode) { };
	virtual void stop_execution(void) { };
	virtual UINT read_io_port(void) { return 0; };
	virtual void write_da_1(UINT value) { };
	virtual void write_da_2(UINT value) { };
	virtual void set_max_counts(UINT counts) { };
	virtual UINT get_counts(void) { return 0; };
	virtual void set_matrix(double m11, double m12, double m21, double m22) { };
	virtual void set_offset(long xoffset, long yoffset) { };
	virtual void goto_xyz(long x, long y, long z) { };
	virtual void goto_xy(long x, long y) { };
	virtual UINT get_hex_version(void) { return 0; };
	virtual void disable_laser(void) { };
	virtual void enable_laser(void) { };
	virtual void stop_list(void) { };
	virtual void restart_list(void) { };
	virtual void get_xyz_pos(long& x, long& y, long& z) { };
	virtual void get_xy_pos(long& x, long& y) { };
	virtual void select_list(UINT list_2) { };
	virtual void z_out(long z) { };
	virtual void set_firstpulse_killer(UINT fpk) { };
	virtual void set_standby(UINT half_period, UINT pulse) { };
	virtual void laser_signal_on(void) { };
	virtual void laser_signal_off(void) { };
	virtual void set_delay_mode(UINT varpoly, UINT directmove3d, UINT edgelevel, UINT minjumpdelay, UINT jumplengthlimit) { };
	virtual void set_piso_control(UINT l1, UINT l2) { };
	virtual void select_status(UINT mode) { };
	virtual void get_encoder(long& zx, long& zy) { };
	virtual void select_cor_table(UINT HeadA, UINT HeadB) { };
	virtual void execute_at_pointer(UINT position) { };
	virtual UINT get_head_status() { return 0; };
	virtual void simulate_encoder(UINT channel) { };
	virtual void set_hi(double galvogainx, double galvogainy, long galvooffsetx, long galvooffsety) { };
	virtual void release_wait(void) { };
	virtual UINT get_wait_status(void) { return 0; };
	virtual void set_control_mode(UINT mode) { };
	virtual void set_laser_mode(UINT mode) { };
	virtual void set_ext_start_delay(long delay, UINT encoder) { };
	virtual void home_position(long xhome, long yhome) { };
	virtual void set_rot_center(long center_x, long center_y) { };
	virtual void dsp_start(void) { };
	virtual void write_da_x(UINT x, UINT value) { };
	virtual UINT read_ad_x(UINT x) { return 0; };
	virtual UINT read_pixel_ad(UINT pos) { return 0; };
	virtual long get_z_distance(long x, long y, long z) { return 0; };
	virtual UINT get_io_status(void) { return 0; };
	virtual double get_time(void) { return 0; };
	virtual void set_defocus(long value) { };
	virtual void set_softstart_mode(UINT mode, UINT number, UINT resetdelay) { };
	virtual void set_softstart_level(UINT index, UINT level) { };
	virtual UINT get_serial_number(void) { return 0; };
	virtual UINT get_board_version(void) { return 0; };
	virtual UINT auto_cal(UINT command) { return 0; };
	virtual UINT get_list_space(void) { return 0; };
	virtual long get_value(UINT signal) { return 0; };
	// RTC3 only
	//virtual void set_bases(UINT base) { };
	virtual void control_command_RTC3(UINT channel, UINT data) { };
	// RTC4
	virtual void get_waveform_RTC4(UINT channel, UINT istop, signed short *memptr) { };
	virtual void measurement_status(UINT& busy, UINT& position) { };
	virtual void set_trigger(UINT sampleperiod, UINT signal1, UINT signal2) { };
	virtual void arc_rel(long dx, long dy, double angle) { };
	virtual void arc_abs(long x, long y, double angle) { };
	virtual void control_command(UINT axis, UINT data) { };

	//===============================================================================================
	// Board-Index-independent
	//
	virtual UINT get_dll_version(void) { return 0; };

	virtual UINT count_cards(void) { return 0; };

};
