/*
 * datetime.h v0.1
 *
 * A SIMPLE AND COMPLEMENTARY LIBRARY FOR THE 'time.h' C-NATIVE LIB.
 *
 * Author: Prof. MSc. Adriano Antunes Prates
 * https://github.com/adrianoifnmg
 *
 * New type added: datetime
 *
 * Functions: dt_now() => gets a current datetime 
 * 		      dt_create("dmyhns",...) => creates a datetime with respective arguments
 *            dt_rand_date() => creates a random date (in current year)
 *            dt_rand_time() => creates a random time (in current day)
 *            dt_rand_birth(N) => creates a random datetime (for least N years)
 *            dt_print() => prints datetime in format "DD/MM/YYYY HH:MM:SS"
 *
 * To install:
 * sudo cp datetime.h /usr/include
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

//'datetime' is an user-defined type that contains the date and time information.
//see all atributes bellow...
typedef struct _dt_ datetime;

//Gets a 'datetime' struct with the current date and time.
datetime dt_now(); 

//Creates and returns a 'datetime' struct with the respective arguments sent.
//The str string recognizes the following characters: "dmyhns"
//You can send any sub-group of these characters and in any order, followed by their values.
datetime dt_create(const char *str,...); 

//Gets a 'datetime' struct with date and time randomly generated (on the current year).
datetime dt_rand_date();

//Gets a 'datetime' struct with only time randomly generated (on the current day).
datetime dt_rand_time();

//Gets a 'datetime' struct with date and time randomly generated (for the least Y years).
datetime dt_rand_birth(int Y);

//Print a 'datetime' struct in format "DD/MM/YYYY HH:MM:SS"
void dt_print(datetime dt);

typedef struct _dt_{
    int sec;     /* seconds,  range 0 to 59 */
    int min;     /* minutes, range 0 to 59 */
    int hour;    /* hours, range 0 to 23 */
    int day;     /* day of the month, range 1 to 31 */
    int mon;     /* month, range 1 to 12  */
    int year;    /* The number of years  */
    int wday;    /* day of the week, range 0 to 6  */
    int yday;    /* day in the year, range 0 to 365 */
    int isdst;   /* daylight saving time */
}datetime;

datetime __make_datetime(time_t convert){
    struct tm *info = localtime(&convert);
    datetime dt;
    dt.sec = info->tm_sec;
    dt.min = info->tm_min;
    dt.hour = info->tm_hour;
    dt.day = info->tm_mday;
    dt.mon = 1+info->tm_mon;
    dt.year = 1900+info->tm_year;
    dt.wday = info->tm_wday;
    dt.yday = info->tm_yday;
    dt.isdst = info->tm_isdst;
    return dt;
}

datetime dt_now(){
    time_t now = time(NULL);
    return __make_datetime(now);
}

datetime dt_create(const char *str,...){
	int args_values[] = {-1,-1,-1,-1,-1,-1};
	int order_values[] = {-1,-1,-1,-1,-1,-1};
	int qt_args = strlen(str);
	va_list _args;
	va_start(_args, str);
	for(int i=0; i<qt_args; i++)
		args_values[i] = va_arg(_args, int);
	va_end(_args);
	
	for(int i=0; i<qt_args; i++)
		switch(str[i]){
			case 'd':	//day
					  if(args_values[i]>=1 && args_values[i]<=31)
						order_values[0] = args_values[i];
					  break;
			case 'm':	//month
					  if(args_values[i]>=1 && args_values[i]<=12)
						order_values[1] = args_values[i];
					  break;
			case 'y':	//year
					  if(args_values[i]>=1900 && args_values[i]<=2200)
						order_values[2] = args_values[i];
					  break;
			case 'h':	//hours
					  if(args_values[i]>=0 && args_values[i]<=23)
						order_values[3] = args_values[i];
					  break;
			case 'i':	//mInutes
			case 'n':	//miNutes
					  if(args_values[i]>=0 && args_values[i]<=59)
						order_values[4] = args_values[i];
					  break;
			case 's':	//seconds
					  if(args_values[i]>=0 && args_values[i]<=59)
						order_values[5] = args_values[i];
					  break;
		};
	
	time_t now = time(NULL);
	struct tm* _dt = localtime(&now);
	_dt->tm_mday = (order_values[0]>0)? order_values[0] : _dt->tm_mday;
	_dt->tm_mon =  (order_values[1]>0)? order_values[1]-1 : _dt->tm_mon;
	_dt->tm_year = (order_values[2]>1900)? order_values[2]-1900 : _dt->tm_year;
	_dt->tm_hour = (order_values[3]>=0)? order_values[3] : 0;
	_dt->tm_min =  (order_values[4]>=0)? order_values[4] : 0;
	_dt->tm_sec =  (order_values[5]>=0)? order_values[5] : 0;
	_dt->tm_isdst = -1; //disable daylight saving time (horário de verão)
	time_t temp = mktime(_dt);
	return __make_datetime(temp);
}

datetime dt_rand_date(){
    time_t now = time(NULL);
    struct tm *day = localtime(&now);
    //set first time as 00:00:00
    day->tm_sec = day->tm_min = day->tm_hour = 0;
    //set first day as 01/01/{Current_Year}
    day->tm_mday = day->tm_mon = 1;
    time_t init_date = mktime(day);
    //set last time as 23:59:59
    day->tm_sec = day->tm_min = 59;
    day->tm_hour = 23;
    //set last day as 31/12/{Current_Year}
    day->tm_mday = 31;
    day->tm_mon = 11; //tm_mon range is 0<=>11
	time_t end_date = mktime(day);
	long int random_date = init_date + rand()%(end_date-init_date);
	return __make_datetime(random_date);
}

datetime dt_rand_time(){
    time_t now = time(NULL);
    struct tm *time_now = localtime(&now);
    //set first time as 00:00:00
    time_now->tm_sec = time_now->tm_min = time_now->tm_hour = 0;
    time_t init_time = mktime(time_now);
    //set last time as 23:59:59
    time_now->tm_sec = time_now->tm_min = 59;
    time_now->tm_hour = 23;
    time_t end_time = mktime(time_now);
	long int random_time = init_time + rand()%(end_time-init_time);
	return __make_datetime(random_time);
}

datetime dt_rand_birth(int y){
    time_t now = time(NULL);
    struct tm *day = localtime(&now);
    //set first time as 00:00:00
    day->tm_sec = day->tm_min = day->tm_hour = 0;
    //set first day as 01/01/{CURRENT_YEAR - Y}
    day->tm_mday = day->tm_mon = 1;
    day->tm_year -= y;
    time_t init_date = mktime(day);
	time_t end_date = time(NULL);
	long int random_date = init_date + rand()%(end_date-init_date);
	return __make_datetime(random_date);
}

void dt_print(datetime dt){
	printf("%02d/%02d/%04d %02d:%02d:%02d",
		dt.day,
		dt.mon,
		dt.year,
		dt.hour,
		dt.min,
		dt.sec
	);
}
