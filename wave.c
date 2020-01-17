#include "wave.h"

/*
#define FREQUENCE_PAR_SECOND = 44100

typedef struct entete_s  {
        char          RIFF[4] ;
        int32_t       taille_riff ;
        char          WAVE[4] ;
        char          fmt_[4] ;
        int32_t       seize_fmt ;
        int16_t       format ;
        int16_t       nb_canaux_c ;
        int32_t       frequence_f ;
        int32_t       oct_sec_t ;
        int16_t       oct_bloc_b ;
        int32_t       precision_p ;
        char          DATA[4] ;
        int32_t       taille_data ;
} entete_t ;


typedef struct wave_s {
        entete_t  en_tete ;
        int32_t   nb_bloc;
        FILE*     file ;
} wave_t;
*/

wave_t* wave_open(const char* fname){
    FILE* file = fopen(fname, "w+");
    wave_t my_file;
    int frequence = FREQUENCE_PAR_SECOND;

    strncpy(my_file.RIFF, "RIFF", 4);
    strncpy(my_file.WAVE, "WAVE", 4);
    strncpy(my_file.fmt_, "fmt ", 4);
    strncpy(my_file.DATA, "data", 4);

    my_file.taille_riff = 0;
    my_file.seize_fmt = 16;
    my_file.format = 1;
    my_file.nb_canaux_c = 1;
    my_file.frequence_f = frequence;
    my_file.oct_sec_t = frequence * (precision / 8);
    my_file.oct_bloc_b = precision / 8;
    my_file.precision = precision;
    my_file.taille_data = 0;

    if(!file)
        return (0);
    fwrite(&my_file, sizeof(my_file), 1, file);
    fflush(file);

    return (my_file);
}
wave_t* wave_create(const char* fname, size_t f, size_t p, size_t c, size_t B);

// Destructors

void wave_close(wave_t* wave);

// Getters / Setters

void wave_print_info(wave_t* wave){
 printf("Number of channels: %lu\n", wave.en_tete.nb_canaux_c);
  printf("Sample rate: %lu\n", wave.en_tete.frequence_f);
  printf("Precision: %lu\n", wave.en_tete.precision_p);
  printf("Number of samples: %lu\n", wave.nb_bloc*wave.en_tete.frequence_f );
}

size_t wave_get_c(wave_t* wave){
    return wave.en_tete.nb_canaux_c;
}

size_t wave_get_f(wave_t* wave){
    return wave.en_tete.frequence_f;
}


size_t wave_get_p(wave_t* wave){
    return wave.en_tete.precision_p;
}


size_t wave_get_D(wave_t* wave){
    return wave.en_tete.taille_data ;
}


size_t wave_get_b(wave_t* wave){
    return wave.en_tete.oct_bloc_b*wave.en_tete.frequence_f;
}


size_t wave_get_r(wave_t* wave){
    return wave.en_tete.nb_canaux_c;
}


size_t wave_get_S(wave_t* wave){
    return wave.en_tete.taille_riff - 8;
}


size_t wave_get_B(wave_t* wave){
    return  wave.nb_bloc*wave.en_tete.frequence_f;
}


void wave_set(wave_t* w, size_t ch, size_t k, double sig);
double wave_get(wave_t* w, size_t ch, double s);
