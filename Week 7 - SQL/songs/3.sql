-- write a SQL query to list the names of the top 5 longest songs, in descending order of length
select name from songs order by duration_ms desc limit 5;