package mechanic

import (
	"github.com/cbuschka/config4go"
	"os"
	"strings"
)

type Config struct {
	logFile string
}

func GetConfig() (*Config, error) {
	envRootDir := os.Getenv("MECHANIC_ROOT_DIR")

	config := Config{logFile: envRootDir + "/var/log/mechanic.log"}
	var configMap map[string]string
	mechanicConfPath := envRootDir + "/etc/mechanic.conf"
	if _, statErr := os.Stat(mechanicConfPath); statErr == nil {
		config, err := config4go.NewConfigReader().ReadConfigFromFile(mechanicConfPath)
		if err != nil {
			return nil, err
		}

		configMap = config.ToMap()
	}

	config.logFile = valueFor("logFile", configMap, config.logFile, envRootDir)

	return &config, nil
}

func valueFor(key string, configMap map[string]string, defaultValue string, envRootDir string) string {
	value := configMap[key]
	if value == "" {
		value = defaultValue
	}

	value = strings.Replace(value, "${MECHANIC_ROOT_DIR}", envRootDir, -1)

	return value
}
