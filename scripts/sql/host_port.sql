CREATE TABLE Host_Port (
  id SERIAL PRIMARY KEY,
  host_id INTEGER NOT NULL,
  port_id INTEGER NOT NULL,

  FOREIGN KEY (host_id) REFERENCES Host(id),
  FOREIGN KEY (port_id) REFERENCES Port(id)
);

CREATE FUNCTION add_host_port(host_id_ INTEGER, port_id_ INTEGER)
RETURNS INTEGER
AS $$
DECLARE
  result INTEGER;
BEGIN
  SELECT id INTO result FROM Host_Address
    WHERE host_id = host_id_
    AND port_id = port_id_;

  IF NOT FOUND THEN
    INSERT INTO Host_Address(host_id, port_id)
    VALUES (host_id_, port_id_)
    RETURNING id INTO result;
  END IF;

  return result;
END
$$ LANGUAGE 'plpgsql';