CREATE TABLE Host (
  id SERIAL PRIMARY KEY,
  address VARCHAR(15) NOT NULL,
  hostname VARCHAR(255) NOT NULL
);

CREATE FUNCTION add_host(address_ VARCHAR(15), hostname_ VARCHAR(255))
RETURNS INTEGER
AS $$
DECLARE
  result INTEGER;
BEGIN
  SELECT id INTO result FROM Host
    WHERE address = address_
    AND hostname = hostname_;
    
  IF NOT FOUND THEN
    INSERT INTO Host(address, hostname) VALUES (address_, hostname_)
    RETURNING id INTO result;
  END IF;
  
  RETURN result;
END
$$ LANGUAGE 'plpgsql';