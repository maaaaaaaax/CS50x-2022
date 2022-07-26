-- Keep a log of any SQL queries you execute as you solve the mystery.

-- learn the naming conventions of streets in crime scene reports
select * from crime_scene_reports limit 5;

-- review crime scene reports for Humphrey Street
select * from crime_scene_reports where street = "Humphrey Street";

-- review crime scene reports for Humphrey Street from July 28, 2021
select * from crime_scene_reports where street = "Humphrey Street" and year = 2021 and month = 7 and day = 28;

-- focus on the crime scene repot that mentions the CS50 duck
select * from crime_scene_reports where street = "Humphrey Street" and year = 2021 and month = 7 and day = 28 and description like "%CS50%";
-- description: Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-- look into the interviews
select * from interviews where year = 2021 and month = 7 and day = 28;

-- focus on interviews that mention bakery
select * from interviews where year = 2021 and month = 7 and day = 28 and transcript like "%bakery%";

/* Results
| id  |  name   | year | month | day | transcript                                                                                                                                                      |
+-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 161 | Ruth    | 2021 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| 162 | Eugene  | 2021 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| 163 | Raymond | 2021 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
| 193 | Emma    | 2021 | 7     | 28  | I'm the bakery owner, and someone came in, suspiciously whispering into a phone for about half an hour. They never bought anything.
*/

/* What we learned:
• The CS50 duck was stolen on 7/28/21 on Humphrey Street at 10:15 AM
• The thief withdrew money from the ATM on Leggett Street the morning of 7/28/21
• Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
  If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
*/

-- learn the naming conventions of streets in atm_transactions
select * from atm_transactions limit 10;
select * from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = "Leggett Street";
select * from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = "Leggett Street" and transaction_type = "withdraw";
select distinct account_number from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = "Leggett Street" and transaction_type = "withdraw";

select person_id from bank_accounts where account_number in (
  select distinct account_number from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = "Leggett Street" and transaction_type = "withdraw"
);

-- Names of people who withdrew money from the ATM on Leggett Street on 7/28/2021
select name from people where id in (
  select person_id from bank_accounts where account_number in (
    select distinct account_number from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = "Leggett Street" and transaction_type = "withdraw"
  )
);

-- Names of people who withdrew money from the ATM on Leggett Street on 7/28/2021:
-- Kenny, Iman, Benista, Taylor, Brooke, Luca, Diana, Bruce

-- List all the cars that left the parking lot within 10 minutes of the theft
select license_plate from bakery_security_logs where year = 2021 and month = 7 and day = 28 and hour = 10 and minute > 14 and minute < 26 and activity = "exit";
-- return the names of people who own these cars:

select * from phone_calls where year = 2021 and month = 7 and day = 28;
-- caller should be among these:
select * from phone_calls where year = 2021 and month = 7 and day = 28 and duration < 60;

/*
Limit the list of suspects based on:
• The thief withdrew money from the ATM on Leggett Street the morning of 7/28/21.
• As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
*/
select name from people where phone_number in (
  select caller from phone_calls where year = 2021 and month = 7 and day = 28 and duration < 60
) and id in (
  select person_id from bank_accounts where account_number in (
    select distinct account_number from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = "Leggett Street" and transaction_type = "withdraw"
  )
) and license_plate in (
  select license_plate from bakery_security_logs where year = 2021 and month = 7 and day = 28 and hour = 10 and minute > 14 and minute < 26 and activity = "exit"
);
-- Suspects: Diana, Bruce

/*
Limit suspects based on:
"I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow"
*/
select * from flights where year = 2021 and month = 7 and day = 28;
-- figure out Fiftyville's airport_id
select * from airports;

select id from airports where city = "Fiftyville";

-- return information on the earliest flight out of Fiftyville 7/29/21
select * from flights where year = 2021 and month = 7 and day = 29 and origin_airport_id in (
  select id from airports where city = "Fiftyville"
) order by hour asc, minute asc limit 1;

-- decide the destination of the thief:
select destination_airport_id from flights where year = 2021 and month = 7 and day = 29 and origin_airport_id in (
  select id from airports where city = "Fiftyville"
) order by hour asc, minute asc limit 1;

select city from airports where id in (
  select destination_airport_id from flights where year = 2021 and month = 7 and day = 29 and origin_airport_id in (
    select id from airports where city = "Fiftyville"
  ) order by hour asc, minute asc limit 1
);
-- The thief escaped to New York City

-- get the passport numbers of all the people of the departing flight
select passport_number from passengers where flight_id in (
  select id from flights where year = 2021 and month = 7 and day = 29 and origin_airport_id in (
    select id from airports where city = "Fiftyville"
  ) order by hour asc, minute asc limit 1
);

-- filter existing list of suspects to know include passport number
select name from people where phone_number in (
  select caller from phone_calls where year = 2021 and month = 7 and day = 28 and duration < 60
) and id in (
  select person_id from bank_accounts where account_number in (
    select distinct account_number from atm_transactions where year = 2021 and month = 7 and day = 28 and atm_location = "Leggett Street" and transaction_type = "withdraw"
  )
) and license_plate in (
  select license_plate from bakery_security_logs where year = 2021 and month = 7 and day = 28 and hour = 10 and minute > 14 and minute < 26 and activity = "exit"
) and passport_number in (
  select passport_number from passengers where flight_id in (
    select id from flights where year = 2021 and month = 7 and day = 29 and origin_airport_id in (
      select id from airports where city = "Fiftyville"
    ) order by hour asc, minute asc limit 1
  )
);
-- Bruce stole the duck!

-- figure out who Bruce called when he left the bakery
select phone_number from people where name = "Bruce";

select * from phone_calls where year = 2021 and month = 7 and day = 28 and duration < 60 and caller in (
  select phone_number from people where name = "Bruce"
);

select receiver from phone_calls where year = 2021 and month = 7 and day = 28 and duration < 60 and caller in (
  select phone_number from people where name = "Bruce"
);

select name from people where phone_number in (
  select receiver from phone_calls where year = 2021 and month = 7 and day = 28 and duration < 60 and caller in (
    select phone_number from people where name = "Bruce"
  )
);
-- Robin was the accomplice