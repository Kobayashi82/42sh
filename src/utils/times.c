/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   times.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:54:55 by vzurera-          #+#    #+#             */
/*   Updated: 2026/01/07 23:49:36 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma region "Includes"

	#include "utils/utils.h"

	#include <sys/time.h>

#pragma endregion

#pragma region "Format TimeStamp"

	#pragma region "Is Leap Year"

		//	Check if a year is leap year
		static int is_leap_year(int year) { return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)); }

	#pragma endregion

	#pragma region "Epoch to Local"

		//	Decompose timestamp into local date and time
		static void epoch_to_local(time_t timestamp, int *year, int *month, int *day, int *hour, int *minute, int *second) {
			int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
			int days = timestamp / 86400;

			*year = 1970;
			while (1) {
				int year_days = is_leap_year(*year) ? 366 : 365;
				if (days < year_days) break;
				days -= year_days; (*year)++;
			}

			if (is_leap_year(*year))	days_in_month[1] = 29;
			else						days_in_month[1] = 28;

			*month = 0;
			while (1) {
				if (days < days_in_month[*month]) break;
				days -= days_in_month[*month]; (*month)++;
			} (*month)++;

			*day = days + 1;

			int remaining_seconds = timestamp % 86400;
			*hour = remaining_seconds / 3600;
			remaining_seconds %= 3600;
			*minute = remaining_seconds / 60;
			*second = remaining_seconds % 60;
		}

	#pragma endregion

	#pragma region "Format TimeStamp"

		//	Format a timestamp to a string as ("dd/mm/yyy HH:MM:SS")
		char *format_timestamp(long timestamp) {
			if (timestamp == (time_t)-1) return (NULL);
			int year, month, day, hour, minute, second, i = 0;
			epoch_to_local(timestamp, &year, &month, &day, &hour, &minute, &second);

			char *formatted_time = malloc(20);

			// Day (dd)
			formatted_time[i++] = '0' + (day / 10);
			formatted_time[i++] = '0' + (day % 10);
			formatted_time[i++] = '/';

			// Month (mm)
			formatted_time[i++] = '0' + (month / 10);
			formatted_time[i++] = '0' + (month % 10);
			formatted_time[i++] = '/';

			// Year (YYYY)
			formatted_time[i++] = '0' + ((year / 1000) % 10);
			formatted_time[i++] = '0' + ((year / 100) % 10);
			formatted_time[i++] = '0' + ((year / 10) % 10);
			formatted_time[i++] = '0' + (year % 10);
			formatted_time[i++] = ' ';

			// Hour (HH)
			formatted_time[i++] = '0' + (hour / 10);
			formatted_time[i++] = '0' + (hour % 10);
			formatted_time[i++] = ':';

			// Minute (MM)
			formatted_time[i++] = '0' + (minute / 10);
			formatted_time[i++] = '0' + (minute % 10);
			formatted_time[i++] = ':';

			// Second (SS)
			formatted_time[i++] = '0' + (second / 10);
			formatted_time[i++] = '0' + (second % 10);

			formatted_time[i] = '\0';

			return (formatted_time);
		}

	#pragma endregion

#pragma endregion

#pragma region "Get TimeStamp"

	//	Get the timestamp from a date in format ("dd/mm/yyy HH:MM:SS") or from now if date is NULL
	long get_timestamp(const char *date) {
		if (!date) { int gmt = 1;	//	GMT+1 for Spain (i have to do it like this because limitation in functions... thanks 42)
			struct timeval tv;
			gettimeofday(&tv, NULL);
			return (tv.tv_sec + (gmt * 3600));
		}
		int day, month, year, hour, minute, second;
		int days_in_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		day = (date[0] - '0') * 10 + (date[1] - '0');
		month = (date[3] - '0') * 10 + (date[4] - '0');
		year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');
		hour = (date[11] - '0') * 10 + (date[12] - '0');
		minute = (date[14] - '0') * 10 + (date[15] - '0');
		second = (date[17] - '0') * 10 + (date[18] - '0');

		if (month < 1 || month > 12 || day < 1 || day > 31 || hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) return (-1);

		int days = 0;
		for (int y = 1970; y < year; y++) days += is_leap_year(y) ? 366 : 365;
		if (is_leap_year(year)) days_in_month[1] = 29;
		for (int m = 0; m < month - 1; m++) days += days_in_month[m];

		days += day - 1;

		time_t timestamp = (time_t)days * 86400 + hour * 3600 + minute * 60 + second;
		return (timestamp);
	}

#pragma endregion

// const char *date = "01/2/2024 14:16:33";
// char *current_date = format_timestamp(get_timestamp(NULL));
// if (current_date) printf("%s\n", current_date);
// return (free(current_date), 0);
