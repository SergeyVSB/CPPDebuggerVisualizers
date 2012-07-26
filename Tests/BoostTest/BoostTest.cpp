#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/variant.hpp>
#include <boost/any.hpp>
#include <boost/optional.hpp>
#include <boost/bimap.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/date_time.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/chrono.hpp>

class Data
{
	int s;
	float f;
};

void mallocDeleter(Data*& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr=NULL;
	}
}


void TestPointerContainerLibrary()
{
	boost::ptr_array<boost::nullable<Data>, 10> b;
	b.replace(0, new Data());
	for(auto it = b.begin(); it!=b.end();it++)
		(*it);
	boost::ptr_vector<boost::nullable<Data>> d;
	d.push_back(new Data());
	d.push_back(0);
	for(auto it = d.begin(); it!=d.end();it++)
		(*it);
	boost::ptr_map<int, Data> e;
	int ival = 0;
	e.insert(ival, new Data());
	for(auto it = e.begin(); it!=e.end();it++)
		(*it);
	boost::ptr_list<Data> g;
	g.push_back(new Data());
	for(auto it = g.begin(); it!=g.end();it++)
		(*it);
	boost::ptr_deque<Data> h;
	h.push_back(new Data());
	for(auto it = h.begin(); it!=h.end();it++)
		(*it);
	boost::ptr_set<int> i;
	i.insert(new int(5));
	for(auto it = i.begin(); it!=i.end();it++)
		(*it);
	boost::ptr_multimap<int, Data> k;
	k.insert(ival, new Data());
	for(auto it = k.begin(); it!=k.end();it++)
		(*it);
	boost::ptr_multiset<int> l;
	l.insert(new int(5));
	for(auto it = l.begin(); it!=l.end();it++)
		(*it);
}

void TestSmartPointers()
{
	boost::scoped_ptr<Data> ptr(new Data());
	boost::scoped_array<Data> ptrAr(new Data[10]());
	boost::shared_ptr<Data> shPtr(new Data());
	boost::weak_ptr<Data> weakPtr(shPtr);
	boost::shared_ptr<Data> shPtrEx( new Data(), std::ptr_fun(mallocDeleter) );
	boost::shared_array<Data> shPtrAr(new Data[10]());
}

void TestVariantAnyOptional()
{
	boost::variant<int, std::string, Data> var("gdsg");
	var = 4;
	boost::variant<int, std::string, Data, bool> var2(false);
	var2 = 4;

	typedef boost::make_recursive_variant<
		int
		, std::vector< boost::recursive_variant_ >
	>::type int_tree_t;

	std::vector< int_tree_t > subresult;
	subresult.push_back(3);
	subresult.push_back(5);

	std::vector< int_tree_t > result;
	result.push_back(1);
	result.push_back(subresult);
	result.push_back(7);

	int_tree_t vartt;
	vartt = result;
	boost::any valany = 4;
	boost::optional<int> opt = 4;
}

void TestUnordered()
{
	boost::unordered_map<std::string, int> um;
	um["hfh"] = 6;
	um["jgfsjsf"] = 86;
	boost::unordered_multimap<std::string, int> umm;
	umm.insert(std::make_pair(std::string("sdhsh"), 73576));
	umm.insert(std::make_pair(std::string("sdhsh"), 34578));
	umm.insert(std::make_pair(std::string("jfsg"), 02));
	auto p = *um.begin();
	boost::unordered_set<int> us;
	us.insert(5);
	us.insert(6543765);
	us.insert(468);
	boost::unordered_multiset<int> uss;
	uss.insert(63);
	uss.insert(63);
	uss.insert(8568);
}

void TestGregorian() 
{
	using namespace boost::gregorian;
	date weekstart(2002,Feb,1);
	date_duration duration = weeks(1);
	date weekend  = weekstart + weeks(1);
	date d1 = day_clock::local_day();
	date d2 = d1 + days(5);
	if (d2 >= d1) {} //date comparison operators 

	date_period thisWeek(d1,d2);
	if (thisWeek.contains(d1)) {}//do something 

	//iterate and print the week
	day_iterator itr(weekstart);
	while (itr <= weekend) {
		++itr;
	}  
	//input streaming 
	std::stringstream ss("2004-Jan-1");
	date d3;
	ss >> d3;

	//date generator functions 
	//date d5 = next_weekday(d1, Sunday); //calculate Sunday following d1

	//US labor day is first Monday in Sept 
	typedef nth_day_of_the_week_in_month nth_dow;
	nth_dow labor_day(nth_dow::first,Monday, Sep); 
	//calculate a specific date for 2004 from functor 
	date d6 = labor_day.get_date(2004); 
}

void TestPosixTime()
{
	using namespace boost::posix_time;
	boost::gregorian::date d(2002,boost::date_time::Feb,1); //an arbitrary date 
	ptime t1(d, hours(5)+microsec(100)); //date + time of day offset 
	ptime t2 = t1 - minutes(4)+seconds(2);
	ptime now = second_clock::local_time(); //use the clock 
	boost::gregorian::date today = now.date(); //Get the date part out of the time 
	boost::gregorian::date tomorrow = today + boost::gregorian::date_duration(1);
	ptime tomorrow_start(tomorrow); //midnight 

	//input streaming 
	std::stringstream ss("2004-Jan-1 05:21:33.20");
	ss >> t2;

	//starting at current time iterator adds by one hour
	time_iterator titr(now,hours(1)); 
	for (; titr < tomorrow_start; ++titr) {
	}
}

void TestLocalTime()
{
	using namespace boost::local_time;
	using namespace boost::date_time;
	//setup some timezones for creating and adjusting times
	//first time zone uses the time zone file for regional timezone definitions
	tz_database tz_db;
	//tz_db.load_from_file("date_time_zonespec.csv");
	time_zone_ptr nyc_tz = tz_db.time_zone_from_region("America/New_York");
	//This timezone uses a posix time zone string definition to create a time zone
	posix_time_zone timezone("MST-07:00:00");
	time_zone_ptr phx_tz(new posix_time_zone("MST-07:00:00"));

	//local departure time in phoenix is 11 pm on April 2 2005 
	// Note that New York changes to daylight savings on Apr 3 at 2 am)
	local_date_time phx_departure(boost::gregorian::date(2005, Apr, 2), boost::posix_time::hours(23), phx_tz, 
		local_date_time::NOT_DATE_TIME_ON_ERROR);

	boost::posix_time::time_duration flight_length = boost::posix_time::hours(4) + boost::posix_time::minutes(30);
	local_date_time phx_arrival = phx_departure + flight_length;
	//convert the phx time to a nyz time
	local_date_time nyc_arrival = phx_arrival.local_time_in(nyc_tz);
}

int main(int, char*[]) {

	boost::dynamic_bitset<> x(70);
	x[0] = 1;
	x[1] = 1;
	x[4] = 1;

	boost::array<Data, 10> a;
	a[0] = Data();
	for(auto it = a.begin(); it!=a.end();it++)
		(*it);

	boost::tribool b(true);
	b = false;
	b = boost::indeterminate;

	boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

	for ( long i = 0; i < 10000000; ++i )
		std::sqrt( 123.456L ); // burn some time

	boost::chrono::duration<double, boost::giga> sec = boost::chrono::system_clock::now() - start;

	auto tupl = boost::make_tuple(1, "terterwt", true);

	TestPointerContainerLibrary();

	TestSmartPointers();

	TestVariantAnyOptional();

	TestUnordered();

	TestGregorian();

	TestPosixTime();

	TestLocalTime();
	
	return EXIT_SUCCESS;
}