-- write a SQL query that lists the names of songs that are by Post Malone

-- option A:
-- select songs.name from songs join artists on artists.id = songs.artist_id where artists.name = "Post Malone";

-- option B:
select name from songs where artist_id = (select id from artists where name = "Post Malone");