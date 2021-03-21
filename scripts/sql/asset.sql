CREATE TABLE Asset (
  id SERIAL PRIMARY KEY,
  host_id INTEGER NOT NULL,

  FOREIGN KEY (host_id) REFERENCES Host(id)
);

CREATE FUNCTION add_asset(host_id_ INTEGER)
RETURNS INTEGER
AS $$
DECLARE
  result INTEGER;
BEGIN
  SELECT id INTO result FROM Asset
    WHERE host_id = host_id_;

  IF NOT FOUND THEN
    INSERT INTO Asset(host_id) VALUES (host_id_)
    RETURNING id INTO result;
  END IF;

  return result;
END
$$ LANGUAGE 'plpgsql';
