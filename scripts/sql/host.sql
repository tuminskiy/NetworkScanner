CREATE TABLE Host (
  id SERIAL PRIMARY KEY,
  status_id INTEGER NOT NULL,
  mac VARCHAR(255) NOT NULL,
  address VARCHAR(255) NOT NULL,
  vendor VARCHAR(255),
  
  FOREIGN KEY (status_id) REFERENCES Status(id)
);

CREATE FUNCTION add_host(status_id_ INTEGER, mac_ VARCHAR(255), address_ VARCHAR(255), vendor_ VARCHAR(255))
RETURNS INTEGER
AS $$
DECLARE
  result INTEGER;
BEGIN
	SELECT id INTO result FROM Host
    WHERE status_id = status_id_
    AND mac = mac_
    AND address = address_
    AND vendor = vendor_;
    
  IF NOT FOUND THEN
    INSERT INTO Host(status_id, mac, address, vendor)
    VALUES (status_id_, mac_, address_, vendor_)
    RETURNING id INTO result;
  END IF;
  
  RETURN result;
END
$$ LANGUAGE 'plpgsql';