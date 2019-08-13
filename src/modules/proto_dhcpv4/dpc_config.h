#pragma once
/*
 * dpc_config.h
 */

typedef struct dpc_config_s dpc_config_t;

extern dpc_config_t *dpc_config; //!< Global configuration singleton.
#define CONF (*dpc_config)

/*
 *	Main configuration
 */
struct dpc_config_s {
	char const *name;            //!< Name of the daemon.
	bool overwrite_config_name;  //!< Overwrite the configured name, as this
	                             ///< was specified by the user on the command line.
	CONF_SECTION *root_cs;       //!< Root of the main config.

	int debug_level;                 //!< The base debug level.
	bool debug_dev;                  //!< Enable extended debug information for developper.
	bool debug_basename;             //!< Print only file base name.
	bool log_timestamp;              //!< Add timestamp to log messages.

	float progress_interval;         //<! Time interval between periodic progress statistics.
	bool pr_stat_per_input;          //<! Print per-input progress statistics (if multiple input).
	uint32_t pr_stat_per_input_max;  //<! Max number of input items shown in progress statistics.

	bool template;                   //<! Template mode.
	char const *file_input;          //<! Read input items from provided file.
	char const **xlat_files;         //<! Files containing values for xlat "file" expansions.
	uint64_t base_xid;               //<! Base value for xid generated in DHCP packets.
	// Note: This is really a uint32_t, but the config parser requires a uint64_t.

	char const **gateways;           //<! Gateways simulated for sending DHCP packets.

	int32_t packet_trace_lvl;        //<! Packet trace level (0: none, 1: header, 2: and attributes, 3: and encoded hex data).
	bool packet_trace_elapsed;       //<! Prefix packet trace with elapsed time.
	bool packet_trace_timestamp;     //<! Prefix packet trace with current timestamp.
	float request_timeout;           //<! Max time waiting for a reply to a request we've sent.
	uint32_t retransmit_max;         //<! Max retransmissions of a request not replied to (not including first packet).

	float rate_limit;                //<! Limit rate/s of sessions initialized from input (all transactions combined).
	float duration_start_max;        //<! Limit duration for starting new input sessions.
	uint32_t input_num_use;          //<! Max number of uses of each input item (default: unlimited in template mode, 1 otherwise).
	uint32_t session_max_num;        //<! Limit number of sessions initialized from input items.
	uint32_t session_max_active;     //<! Max number of session packets sent concurrently (default: 1).
};

void dpc_config_name_set_default(dpc_config_t *config, char const *name, bool overwrite_config);
dpc_config_t *dpc_config_alloc(TALLOC_CTX *ctx);
int dpc_config_init(dpc_config_t *config, char const *conf_file);
int dpc_config_check(dpc_config_t *config);
void dpc_config_debug(dpc_config_t *config);


/*
 *	Configuration checks.
*/
#define CONF_CHECK_FMT(_fmt, _name, _var, _cond, _expected)\
do {\
	if (!(_cond)) {\
		ERROR("Invalid configuration \"" _name " = %" _fmt "\" (expected: " _expected ")", _var);\
		return 1;\
	}\
} while (0)

#define CONF_CHECK_FLOAT(_name, _var, _cond, _expected)\
	CONF_CHECK_FMT("f", _name, _var, _cond, _expected)

#define CONF_CHECK_UINT(_name, _var, _cond, _expected)\
	CONF_CHECK_FMT("u", _name, _var, _cond, _expected)

#define CONF_CHECK_UINT64(_name, _var, _cond, _expected)\
	CONF_CHECK_FMT(PRIu64, _name, _var, _cond, _expected)
