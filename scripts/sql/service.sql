CREATE TABLE Service (
  id SERIAL PRIMARY KEY,
  name VARCHAR(255) NOT NULL,
  method VARCHAR(255) NOT NULL,
  conf INTEGER NOT NULL
);

CREATE OR REPLACE FUNCTION add_service(name_ VARCHAR(255), method_ VARCHAR(255), conf_ INTEGER)
RETURNS INTEGER
AS $$
DECLARE
  result INTEGER;
BEGIN
  SELECT id INTO result FROM Service
    WHERE name = name_
    AND method = method_
    AND conf = conf_;

  IF NOT FOUND THEN
      INSERT INTO Service(name, method, conf)
      VALUES (name_, method_, conf_)
      RETURNING id INTO result;
  END IF;
      
  RETURN result;
END
$$ LANGUAGE 'plpgsql';