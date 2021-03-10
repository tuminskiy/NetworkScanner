CREATE TABLE ScanResult_Host (
  id SERIAL PRIMARY KEY,
  scanresult_id INTEGER NOT NULL,
  host_id INTEGER NOT NULL,

  FOREIGN KEY (scanresult_id) REFERENCES ScanResult(id),
  FOREIGN KEY (host_id) REFERENCES Host(id)
);

CREATE FUNCTION add_scanresult_host(scanresult_id_ INTEGER, host_id_ INTEGER)
RETURNS INTEGER
AS $$
DECLARE
  result INTEGER;
BEGIN
  SELECT id INTO result FROM ScanResult_Host
    WHERE scanresult_id = scanresult_id_
    AND host_id = host_id_;

  IF NOT FOUND THEN
    INSERT INTO ScanResult_Host(scanresult_id, host_id)
    VALUES (scanresult_id_, host_id_)
    RETURNING id INTO result;
  END IF;

  return result;
END
$$ LANGUAGE 'plpgsql';