/* -------------------------------RCS-------------------------------------- */
/* $Revised: Xuefan $ */
/* $Id: read_test.c,v 1.4 2004/07/08 21:24:07 raffo Exp $ */
/* $Revision: 1.4 $ */
/* -------------------------------RCS-------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include "ndf.h"


static short   dt[16384];
static double  sdt[16384];


void
dump(ndf_handle_ptr_t  hp,
     FILE *  fp)
{
   ndf_status_t      stat;
   
   fflush(fp);
   printf("\nDumping the ndf handle.\n");
   fflush(stdout);
   if ((stat = _ndf_dump(hp, fp, (size_t)0)) <= NDF_ERROR) {
      printf("Couldn't dump the ndf handle.\n");
      fflush(stdout);
      fprintf(stderr, "stat: %d\n", (int)stat);
      fprintf(stderr, "ERROR: couldn't dump ndf handle!\n");
   }
   printf("Dumped the ndf handle.\n");
   fflush(stdout);
   fflush(fp);
   return;
}

int
my_def_for_time_start(ndf_handle_ptr_t  hp, long  cn)
{
   time_t  strt_time;
   long count;
   long start_point;
   
   start_point = 1;
   _ndf_get_epoch_start_time(hp, cn, start_point, &strt_time);
   count = (long)strt_time;
   return count;
   
}


void
ep_channel_data_read_test(ndf_handle_ptr_t  hp, long  cn, long  rn, FILE *  fp)
{
   long    i;
   time_t  strt_time;
   long    aa;
   long    ar;
   size_t    c_len;
   char    comms[MAX_COMMENT_LEN + 1];
   unsigned long    dt_len;
   long count;
   double sub_time;
   double dSclCoef=0;

   // PARAMETER NUM 10
   double db_rn = (double)rn;
   fwrite(&db_rn,sizeof(double),1,fp);

   // PARAMETER NUM 11
   count = my_def_for_time_start(hp, cn);
   _ndf_get_epoch_start_time(hp, cn, rn, &strt_time);
   sub_time = (double)(strt_time - count);
   fwrite(&(sub_time),sizeof(double),1,fp);

   // PARAMETER NUM 12
   _ndf_get_epoch_data_length(hp, cn, rn, &dt_len);
   double len = (double)dt_len;
   fwrite(&len,sizeof(double),1,fp);

   // PARAMETER NUM 13
   // ONLY NEEDED IF SAVING AS SHORTS
   //_ndf_get_scale_coefficient(hp, cn, &dSclCoef);
   //fwrite(&dSclCoef,sizeof(double),1,fp);

   /* getting data in short integers. */
   //_ndf_get_epoch_data(hp, cn, rn,dt);

   /* getting data in Volts at double precision. */
   _ndf_get_scaled_epoch_data(hp, cn, rn, sdt);

   /*for (i = 0; (i < dt_len) && (i < 1000); i++) {
      if ((i % 10) == 0)  fprintf(fp, "\n        ");
      fprintf(fp, "%7d", (int)(dt[i]));
      }
      fprintf(fp, "\n");*/
   /*for (i = 0; (i < dt_len) ; i++) {
     fwrite(&sdt[i],sizeof(double),1,fp);
     }*/

   fwrite(sdt,sizeof(double),dt_len,fp);
}


void
ep_channel_params_read_test(ndf_handle_ptr_t  hp, long  cn, FILE *  fp)
{
   long    wn;
   long    num_wns;
   char    chn_name[CHANNEL_NAME_LEN];
   double  ob_int;
   double  sampl_rt;
   long    d_len;
   double  a_d_u_v;
   double  a_d_l_v;
   double  gain;
   double  low_bd;
   double  high_bd;
   long    st_code;
   double  freq;
   double  p_width;
   double  p_ampl;
   long    a_code;
   long    t_accepts;
   double  a_min;
   double  a_max;
   double  a_vt;
   double  a_bp;

   

   // PARAMETER NUM 3
   _ndf_get_observation_interval(hp, cn, &ob_int);
   fwrite(&ob_int,sizeof(double),1,fp);
   
   // PARAMETER NUM 4
   _ndf_get_sampling_rate(hp, cn, &sampl_rt);
   fwrite(&sampl_rt,sizeof(double),1,fp);
   
   // PARAMETER NUM 5
   _ndf_get_a_d_upper_voltage(hp, cn, &a_d_u_v);
   fwrite(&a_d_u_v,sizeof(double),1,fp);
   
   // PARAMETER NUM 6
   _ndf_get_a_d_lower_voltage(hp, cn, &a_d_l_v);
   fwrite(&a_d_l_v,sizeof(double),1,fp);
   
   // PARAMETER NUM 7
   _ndf_get_amplifier_gain(hp, cn, &gain);
   fwrite(&gain,sizeof(double),1,fp);

   // PARAMETER NUM 8
   _ndf_get_amplifier_low_band(hp, cn, &low_bd);
   fwrite(&low_bd,sizeof(double),1,fp);
   
   // PARAMETER NUM 9
   _ndf_get_amplifier_high_band(hp, cn, &high_bd);
   fwrite(&high_bd,sizeof(double),1,fp);

   /*
   THIS IS NOT NEEDED. THE EMG DATA DOES NOT HAVE THESE PARAMETERS.

   _ndf_get_stimulus_frequency(hp, cn, &freq);
   fwrite(&freq,sizeof(double),1,fp);
   
   _ndf_get_stimulus_duration(hp, cn, &p_width);
   fwrite(&p_width,sizeof(double),1,fp);
   
   _ndf_get_stimulus_amplitude(hp, cn, &p_ampl);
   fwrite(&p_ampl,sizeof(double),1,fp);
   */

   return;
}




void
read_test(ndf_handle_ptr_t  hp,
	  FILE *  fp)
{
   long  cmap[32];
   long  rn;
   long  num_chns;
   long  num_recs;
   long  i;
   ndf_status_t  st;

   // PARAMETER NUM 1
   _ndf_query_number_channels(hp, &num_chns);
   double chns = (double)num_chns;
   fwrite(&chns,sizeof(double),1,fp);

   // PARAMETER NUM 2
   _ndf_query_number_records(hp, &num_recs);
   double recs = (double)num_recs;
   fwrite(&recs,sizeof(double),1,fp);

   _ndf_query_channel_map(hp, cmap);
   for (i = 0; i < num_chns; i++) {
       ep_channel_params_read_test(hp, cmap[i], fp);
   }

   for (i = 0; i < num_chns; i++) {
      printf("  Channel %d.\n", cmap[i]);
      for (rn = 1;rn <= num_recs;rn++) {
	  ep_channel_data_read_test(hp, cmap[i], rn, fp);
      }
      printf("  Channel %d read. %d epochs.\n", cmap[i], num_recs);
      printf("  sizeof(short)=%d sizeof(int)=%d sizeof(float)=%d sizeof(double)=%d.\n",
	  sizeof(short), sizeof(int), sizeof(float), sizeof(double));
   }
}
     


void
main(int argc, char **argv)
{
   FILE *            fp;
   ndf_handle_ptr_t  hp = NULL;
   ndf_status_t      stat;
   char *            fname;
   char *            output;
 
   // The output file name must be an argument.
   output = argv[2]
   fp = fopen(output, "wb");
   if (fp == NULL) {
      fprintf(stderr, "ERROR: couldn't open dump file!\n");
      exit(1);
   }
   fname = argv[1];
   if ((stat = _ndf_open(fname, ndf_read, &hp)) <= NDF_ERROR)  {
       fprintf(stderr, "stat: %d\n", (int)stat);
       fprintf(stderr, "ERROR: couldn't open ndf file '%s'!\n", fname);
       exit(1);
   }

   fprintf(stderr, "READ: ndf file opened successfully.\n");
   read_test(hp, fp);
   fclose(fp);
   if ((stat = _ndf_close(hp)) <= NDF_ERROR)  {
      fprintf(stderr, "stat: %d\n", (int)stat);
      fprintf(stderr, "ERROR: couldn't close ndf file!\n");
      exit(1);
   }
}

