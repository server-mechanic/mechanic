package mechanic

import (
	"github.com/cbuschka/config4go"
	"os"
	"path"
	"strings"
)

type Config struct {
	logFile string
	varDir  string
	etcDir  string
}

func (config *Config) GetInventoryDbPath() string {
	return path.Clean(config.GetVarDir() + "/inventory.db")
}

func (config *Config) GetVarDir() string {
	return config.varDir
}

func (config *Config) GetStateDir() string {
	return config.GetVarDir() + "/state"
}

func (config *Config) GetEtcDir() string {
	return config.etcDir
}

func (config *Config) GetMigrationsDir() string {
	return path.Clean(config.etcDir + "/migration.d")
}
func getDefaults() *Config {
	return &Config{logFile: "/var/log/mechanic.log",
		varDir: "/var/lib/mechanic/",
		etcDir: "/etc/mechanic",
	}
}

func (config *Config) rebase(base string) {
	config.logFile = base + config.logFile
	config.varDir = base + config.varDir
	config.etcDir = base + config.etcDir
}

func GetConfig() (*Config, error) {

	config := getDefaults()

	envRootDir := os.Getenv("MECHANIC_ROOT_DIR")
	if envRootDir != "" {
		config.rebase(envRootDir)
	}

	envEtcDir := os.Getenv("MECHANIC_ETC_DIR")
	if envEtcDir != "" {
		config.etcDir = path.Clean(envEtcDir)
	}

	envVarDir := os.Getenv("MECHANIC_VAR_DIR")
	if envVarDir != "" {
		config.varDir = path.Clean(envVarDir)
	}

	configMap, _ := config.loadConfigAsMap()

	if configMap != nil {
		config.logFile = valueFor("logFile", configMap, config.logFile, envRootDir)
	}

	return config, nil
}

func (config *Config) getMechanicConfPath() string {
	return path.Clean(config.GetEtcDir() + "/../mechanic.conf")
}

func (config *Config) loadConfigAsMap() (map[string]string, error) {
	mechanicConfPath := config.getMechanicConfPath()

	if _, err := os.Stat(mechanicConfPath); err != nil {
		return nil, err
	}

	config4go, err := config4go.NewConfigReader().ReadConfigFromFile(mechanicConfPath)
	if err != nil {
		return nil, err
	}

	return config4go.ToMap(), nil
}

func valueFor(key string, configMap map[string]string, defaultValue string, envRootDir string) string {
	value := configMap[key]
	if value == "" {
		value = defaultValue
	}

	value = strings.Replace(value, "${MECHANIC_ROOT_DIR}", envRootDir, -1)

	return value
}
