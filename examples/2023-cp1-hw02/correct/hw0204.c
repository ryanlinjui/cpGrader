// Credit: 41247004S 廖Ｏ翔

#include <stdio.h>
#include <stdint.h>

int32_t rounded(int32_t rate)
{
	if(rate%10 < 5)
		return rate/10;
	else
		return (rate/10)+1;
}
int32_t int_judge(double input)
{
	if (input-(int32_t)(input) > 0)
		return 1;
	else
		return 0;
}


int main(int argc, char const *argv[])
{
	double i_investment = 0, monthly_investment = -1, s_month = 0, s_year = 0, e_month = 0, e_year = 0;
	double initial_investment = 0, y_rate = 0, m_rate = 0;

	printf("Initial Investment:\t\t");
	scanf("%lf", &i_investment);
	if (int_judge(i_investment))
	{
		printf("Error: please enter a integer.\n");
		return -1;
	}
	if (i_investment < 1 || i_investment > 10000000)
	{
		printf("Error: please enter a integer for Initial Investment between 1~10000000\n");
		return -1;
	}
	initial_investment = i_investment;
	printf("Recurring Monthly Investment:\t");
	scanf("%lf", &monthly_investment);
	if (int_judge(monthly_investment))
	{
		printf("Error: please enter a integer.\n");
		return -1;
	}
	if (monthly_investment < 0 || monthly_investment > 10000000)
	{
		printf("Error: please enter enter a integer for Recurring Monthly Investment between 0~10000000\n");
		return -1;
	}
	printf("Start Month:\t\t\t");
	scanf("%lf", &s_month);
	if (int_judge(s_month))
	{
		printf("Error: please enter a integer.\n");
		return -1;
	}
	if (s_month < 1 || s_month > 12)
	{
		printf("Error: please enter enter a integer for Start Month between 1~12\n");
		return -1;
	}
	printf("Start Year:\t\t\t");
	scanf("%lf", &s_year);
	if (int_judge(s_year))
	{
		printf("Error: please enter a integer.\n");
		return -1;
	}
	if (s_year < 1 || s_year > 10000)
	{
		printf("Error: please enter enter a integer for Start Year between 1~10000\n");
		return -1;
	}
	printf("End Month:\t\t\t");
	scanf("%lf", &e_month);
	if (int_judge(e_month))
	{
		printf("Error: please enter a integer.\n");
		return -1;
	}
	if (e_month < 1 || e_month > 12)
	{
		printf("Error: please enter enter a integer for End Month between 1~12\n");
		return -1;
	}
	printf("End Year:\t\t\t");
	scanf("%lf", &e_year);
	if (int_judge(e_year))
	{
		printf("Error: please enter a integer.\n");
		return -1;
	}
	if (e_year < 1 || e_year > 10000)
	{
		printf("Error: please enter enter a integer for End Year between 1~10000\n");
		return -1;
	}
	printf("Annual Rate of Return (%%):\t");
	scanf("%lf", &y_rate);
	if (int_judge(y_rate))
	{
		printf("Error: please enter a integer.\n");
		return -1;
	}
	if (y_rate < 1 || y_rate > 100)
	{
		printf("Error: please enter enter a integer for Annual Rate of Return between 1~10000000\n");
		return -1;
	}
	if (e_year-s_year < 0 || (e_year-s_year == 0 && e_month-s_month <= 0) )
	{
		printf("Error: Time range must larger than a month.\n");
		return -1;
	}
	m_rate = (y_rate/12)/100;
//	printf("%lf\n", m_rate);
	printf("--- Output ---\n");

	double total = initial_investment, investment = initial_investment;
	int32_t month = s_month;

	for (int32_t year = s_year; year <= e_year; year++)
	{
		for (; month <= 12; month++)
		{
			int32_t rate = (int32_t)(((total-initial_investment)/total)*100000);
			rate = rounded(rate);

			printf("%d.", year);
			if (month < 10)
				printf("0");		 
			if (initial_investment < 1000000000000000 && total < 1000000000000000)
				if (rate%10 > 0 )
					printf("%d) %.0lf/%.0lf/%.0lf/%.2lf%%\n", month, initial_investment, total, total-initial_investment, rate/100.0);
				else if (rate % 100 > 0)
					printf("%d) %.0lf/%.0lf/%.0lf/%.0lf.%.0lf%%\n", month, initial_investment, total, total-initial_investment, (double)(rate/100), (double)(rate%100)/10);
				else
					printf("%d) %.0lf/%.0lf/%.0lf/%.0lf%%\n", month, initial_investment, total, total-initial_investment, (double)(rate/100));
			else
				printf("%d) */*/*/*%%\n", month);

			if (year == e_year && month == e_month-1)
			{
				break;
			}
			investment = total*(1+m_rate);
			total = investment + monthly_investment;
			initial_investment += monthly_investment;
		}
		month = 1;
	}

	return 0;
}