CREATE TABLE Status (
  id SERIAL PRIMARY KEY,
  state VARCHAR(255) NOT NULL,
  reason VARCHAR(255) NOT NULL
);

CREATE FUNCTION add_status(state_ VARCHAR(255), reason_ VARCHAR(255))
RETURNS INTEGER
AS $$
DECLARE
  result INTEGER;
BEGIN
	SELECT id INTO result FROM Status
    WHERE state = state_
    AND reason = reason_;
    
  IF NOT FOUND THEN
    INSERT INTO Status(state, reason)
    VALUES (state_, reason_)
    RETURNING id INTO result;
  END IF;
  
  RETURN result;
END
$$ LANGUAGE 'plpgsql';