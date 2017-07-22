
   #ifndef __QPARAM_H__
   #define __QPARAM_H__



#ifdef __cplusplus
extern "C" {
#endif



AERERR_CODE AER_LIBENTRY   aerq_save_param (SHORT *params, char *filename);
AERERR_CODE AER_LIBENTRY   aerq_close_param (PSHORT *params);
AERERR_CODE AER_LIBENTRY   aerq_open_param (char *filename, PSHORT *pParams);
AERERR_CODE AER_LIBENTRY   aerq_write_param (SHORT *params, SHORT paramnum, char *paramval);
AERERR_CODE AER_LIBENTRY   aerq_read_param (SHORT *params, SHORT paramnum, char *paramval);
AERERR_CODE AER_LIBENTRY   aerq_read_axcal(char *config, char *calfile);
AERERR_CODE AER_LIBENTRY   aerq_save_axcal(char *origconfig, char *newconfig, char *calfile);
AERERR_CODE AER_LIBENTRY   aerq_create_default_param_file( CHAR *fname );
AERERR_CODE AER_LIBENTRY   aerq_check_default_param(SHORT *params, SHORT paramnum);
AERERR_CODE AER_LIBENTRY   aerq_create_default_cfg_file( CHAR *fname );
AERERR_CODE AER_LIBENTRY   aerq_get_default_cfg(SHORT paramnum, LPSTR retbuf);
LONG        AER_LIBENTRY   aerq_get_param_size( void );



AERERR_CODE AER_LIBENTRY aerq_read_board_param(BOARD *pb, SHORT paramnum, double *pdparamval);

#ifdef __cplusplus
}
#endif

enum U511_PRM_NUMBERS
{
   U511PRM_AUTOENABLE = 0 , U511PRM_AUTOPROG, U511PRM_AXISCAL,

   U511PRM_PRMFILE, U511PRM_FRMFILE, U511PRM_MCODEFILE, U511PRM_GLBSUBFILE,

   U511PRM_PSOFRMFILE ,

   U511PRM_COM1BAUD   ,U511PRM_COM1DATA  , U511PRM_COM1STOP , U511PRM_COM1PARITY,
   U511PRM_COM1EOS    ,U511PRM_COM1INTIO , U511PRM_COM1ACK  , U511PRM_COM1NAK  ,
   U511PRM_COM1CONFIG,

   U511PRM_COM2BAUD   ,U511PRM_COM2DATA  , U511PRM_COM2STOP , U511PRM_COM2PARITY,
   U511PRM_COM2EOS    ,U511PRM_COM2INTIO ,  U511PRM_COM2ACK, U511PRM_COM2NAK   ,
   U511PRM_COM2CONFIG,

   U511PRM_GPIBADD    ,U511PRM_GPIBEOS  , U511PRM_GPIBPPRBIT , U511PRM_GPIBTIMEOUT,
   U511PRM_GPIBCONFIG,

   U511PRM_JOG1LOW    ,U511PRM_JOG2LOW   ,U511PRM_JOG3LOW   ,U511PRM_JOG4LOW   ,
   U511PRM_JOG1HIGH   ,U511PRM_JOG2HIGH  ,U511PRM_JOG3HIGH  ,U511PRM_JOG4HIGH  ,
   U511PRM_JOG1DIST   ,U511PRM_JOG2DIST  ,U511PRM_JOG3DIST  ,U511PRM_JOG4DIST  ,

// autotune parameters - for internal use only
   U511PRM_ATDIST,    U511PRM_ATBW,      U511PRM_ATFREQ,   U511PRM_ATDAMP,

   U511PRM_SPEAKER,  U511PRM_PASSWORD,

// must be last
   U511PRM_LAST
};

   #endif  //  __QPARAM_H__

