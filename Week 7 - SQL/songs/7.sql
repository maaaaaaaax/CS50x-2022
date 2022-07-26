-- write a SQL query that returns the average energy of songs that are by Drake
select avg(energy) from songs where artist_id=(select id from artists where name = "Drake");