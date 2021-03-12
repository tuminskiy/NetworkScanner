CREATE TABLE Port (
  id SERIAL PRIMARY KEY,
  portid INTEGER NOT NULL,
  protocol VARCHAR(255) NOT NULL,
  service_id INTEGER NOT NULL,
  status_id INTEGER NOT NULL,

  FOREIGN KEY (service_id) REFERENCES Service(id),
  FOREIGN KEY (status_id) REFERENCES Status(id)
);

CREATE FUNCTION add_port(portid_ INTEGER, protocol_ VARCHAR(255), service_id_ INTEGER, status_id_ INTEGER)
RETURNS INTEGER
AS $$
DECLARE
  result INTEGER;
BEGIN
	SELECT id INTO result FROM Port
    WHERE portid = portid_
    AND protocol = protocol_
    AND service_id = service_id_
    AND status_id = status_id_;
  
  IF NOT FOUND THEN
    INSERT INTO Port(portid, protocol, service_id, status_id)
    VALUES (portid_, protocol_, service_id_, status_id_)
    RETURNING id INTO result;
  END IF;
  
  RETURN result;
END
$$ LANGUAGE 'plpgsql';