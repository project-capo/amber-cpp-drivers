#include <log4cxx/propertyconfigurator.h>

#include "HitecController.h"
#include "HitecCommon.h"
#include <google/protobuf/repeated_field.h>

#include <boost/program_options.hpp>
#include <string>
#include <cmath>

using namespace std;
using namespace boost;
using namespace boost::interprocess;
using namespace boost::program_options;
using namespace log4cxx;
using namespace amber;
using namespace google::protobuf;

LoggerPtr HitecController::_logger(Logger::getLogger("Hitec.Controller"));

HitecController::HitecController(int pipeInFd, int pipeOutFd, const char *confFilename) {
    parseConfigurationFile(confFilename);
    _hitecDisabled = false;
    _overheated = false;

    _hitecDriver = new HitecDriver(_configuration);
    _amberPipes = new AmberPipes(this, pipeInFd, pipeOutFd);

    _hitecDriver->initializeDriver();
}

HitecController::~HitecController() {
    LOG4CXX_INFO(_logger, "Stopping controller.");

    delete _hitecDriver;
    delete _amberPipes;
}

void HitecController::handleDataMsg(amber::DriverHdr *driverHdr, amber::DriverMsg *driverMsg) {
    if (_logger->isDebugEnabled()) {
        LOG4CXX_DEBUG(_logger, "Message came");
    }
    if (driverMsg->HasExtension(hitec_proto::setAngleCommand)) {
    	handleSetAngleCommand(driverMsg->MutableExtension(hitec_proto::setAngleCommand));
    } else if (driverMsg->HasExtension(hitec_proto::setSameAngleCommand)) {
        handleSetSameAngleCommand(driverMsg->MutableExtension(hitec_proto::setSameAngleCommand));
    } else if(driverMsg->HasExtension(hitec_proto::setDifferentAnglesCommand)) {
    	handleSetDifferentAngleCommand(driverMsg->MutableExtension(hitec_proto::setDifferentAnglesCommand));
    } else if(driverMsg->HasExtension(hitec_proto::setSpeedCommand)) {
    	handleSetSpeedCommand(driverMsg->MutableExtension(hitec_proto::setSpeedCommand));
    }
	else {
    	throw new HitecUnsupportedCommandException();
    }
}

void HitecController::handleSetAngleCommand(hitec_proto::SetAngle *setAngleCommand) {
    if (_logger->isDebugEnabled()) {
        LOG4CXX_DEBUG(_logger, "Handling setAngleCommand message");
    }

    unsigned int servo_address = setAngleCommand->servoaddress();
    unsigned int angle = setAngleCommand->angle();

    if(!_hitecDisabled) {
    	try {
    		_hitecDriver->setAngle(servo_address, angle);
		} catch (HitecSerialException &e) {
			LOG4CXX_WARN(_logger, "Serial connection error in HitecDriver.setAngle(...).");
		}
    }
}

void HitecController::handleSetSpeedCommand(hitec_proto::SetSpeed *setSpeedCommand) {
    if (_logger->isDebugEnabled()) {
        LOG4CXX_DEBUG(_logger, "Handling setSpeedCommand message");
    }

    unsigned int servo_address = setSpeedCommand->servoaddress();
    unsigned int speed = setSpeedCommand->speed();

    if(!_hitecDisabled) {
    	try {
    		_hitecDriver->setSpeed(servo_address, speed);
		} catch (HitecSerialException &e) {
			LOG4CXX_WARN(_logger, "Serial connection error in HitecDriver.setSpeed(...).");
		}
    }
}

void HitecController::handleSetSameAngleCommand(hitec_proto::SetSameAngle *setSameAngleCommand) {
    if (_logger->isDebugEnabled()) {
        LOG4CXX_DEBUG(_logger, "Handling setSameCommand message");
    }

    RepeatedField<unsigned int> raw_servo_addresses = setSameAngleCommand->servoaddresses();
    int servo_count = raw_servo_addresses.size();
    unsigned int *servo_addresses = NULL;
    raw_servo_addresses.ExtractSubrange(0, servo_count, servo_addresses);
    int angle = setSameAngleCommand->angle();

    if(!_hitecDisabled) {
    	try {
    		_hitecDriver->setSameAngle(servo_addresses, servo_count, angle);
		} catch (HitecSerialException &e) {
			LOG4CXX_WARN(_logger, "Serial connection error in HitecDriver.setSameAngle(...).");
		}
    }
}

void HitecController::handleSetDifferentAngleCommand(hitec_proto::SetDifferentAngles *setDifferentAnglesCommand) {
    if (_logger->isDebugEnabled()) {
        LOG4CXX_DEBUG(_logger, "Handling setDifferentAngleCommand message");
    }

    RepeatedField<unsigned int> raw_servo_addresses = setDifferentAnglesCommand->servoaddresses();
    int servo_count = raw_servo_addresses.size();
    unsigned int *servo_addresses = NULL;
    raw_servo_addresses.ExtractSubrange(0, servo_count, servo_addresses);

    RepeatedField<int> raw_angles = setDifferentAnglesCommand->angles();
    int angles_count = raw_angles.size();
    int *angles = NULL;
    raw_angles.ExtractSubrange(0, angles_count, angles);

    if(servo_count != angles_count) {
    	LOG4CXX_WARN(_logger, "Servo count and angles count not equal in HitecDriver.setDifferentAngles(...).");
    }

    if(!_hitecDisabled) {
    	try {
    		_hitecDriver->setDifferentAngles(servo_addresses, servo_count, angles);
		} catch (HitecSerialException &e) {
			LOG4CXX_WARN(_logger, "Serial connection error in HitecDriver.setDifferentAngles(...).");
		}
    }
}

void HitecController::parseConfigurationFile(const char *filename) {
    LOG4CXX_INFO(_logger, "Parsing configuration file: " << filename);

    _configuration = new HitecConfiguration();

    options_description desc("Hitec options");
    desc.add_options()
            ("Hitec.uart_port", value<string>(&_configuration->uart_port)->default_value("/dev/ttyO3"))
            ("Hitec.uart_speed", value<unsigned int>(&_configuration->uart_speed)->default_value(38400));


    variables_map vm;

    try {
        store(parse_config_file<char>(filename, desc), vm);
        notify(vm);
    } catch (std::exception& e) {
        LOG4CXX_ERROR(_logger, "Error in parsing configuration file: " << e.what());
        exit(1);
    }
}

void HitecController::runAmberPipesProcess() {
	_amberPipes->operator ()();
}

void HitecController::handleClientDiedMsg(int clientID) {
	LOG4CXX_INFO(_logger, "Client " << clientID << " died, no response necessary.");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        return 1;
    }

    const char *confFile = argv[1];
    const char *logConfFile = argv[2];

    PropertyConfigurator::configure(logConfFile);

    // STDIN_FD = 0, STDOUT_FD = 1
    // pipe_in_fd = 0, pipe_out_fd = 1
    LoggerPtr logger(Logger::getLogger("main"));

    LOG4CXX_INFO(logger, "-------------");
    LOG4CXX_INFO(logger, "Creating controller, config_file: " << argv[1] << ", log_config_file: " << argv[2]);

    HitecController controller(0, 1, confFile);
    controller.runAmberPipesProcess();
}
