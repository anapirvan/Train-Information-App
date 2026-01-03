To run the project, simply use the provided Makefile.

The client and server files contain the main code, which facilitates communication between clients and a server. The server can be run locally or on a remote machine (I have tested it on a university server and it works).

The project uses two XML files:

log.xml – stores user data for login and usage of the app.

trenuri.xml – contains all information about the trains, including their IDs, routes, timetables, and delays.

The app implements the following commands:

login username password – logs in a user.

logout – logs out the current user.

quit – exits the app.

mersul_trenurilor – displays all train information (user must be logged in).

status_plecari – shows information about trains departing in the next hour (user must be logged in).

status_sosiri – shows information about trains arriving in the next hour (user must be logged in).

update_plecare id delay – updates the departure delay of a train in the XML file (user must be logged in).

update_sosire id delay – updates the arrival delay of a train in the XML file (user must be logged in).

The server is multithreaded and also has two workers that process commands from the queue using mutexes to ensure thread-safe access.
