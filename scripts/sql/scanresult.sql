CREATE TABLE ScanResult (
  id SERIAL PRIMARY KEY,
  start_dt TIMESTAMP NOT NULL,
  end_dt TIMESTAMP NOT NULL
);

CREATE FUNCTION add_scanresult(start_dt_ TIMESTAMP, end_dt_ TIMESTAMP)
RETURNS INTEGER
AS $$
DECLARE
  result INTEGER;
BEGIN
  INSERT INTO ScanResult(start_dt, end_dt)
  VALUES (start_dt_, end_dt_) RETURNING id INTO result;

  return result;
END
$$ LANGUAGE 'plpgsql';