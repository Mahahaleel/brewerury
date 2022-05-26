DROP TABLE IF EXISTS brew_parameters;

CREATE TABLE brew_parameters (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    created TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    brew_temperature FLOAT NOT NULL,
    measure_interval INT NOT NULL,
    fan_cycle_delay INT
);

DROP TABLE IF EXISTS arduino_power;

CREATE TABLE arduino_power (
    parameter_name VARCHAR(250) PRIMARY KEY,
    description VARCHAR(250) NOT NULL,
    value VARCHAR(50) NOT NULL
);