#ifndef __MAIN_H__
#define __MAIN_H__

/*INCLUDE STANDARD LIBRARIES*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <arpa/inet.h>    
#include <sys/types.h>  
#include <time.h> 
#include<sys/wait.h> 

/*INCLUDE USER-DEFINED HEADERS*/
#include "encryption.h"

/*DEFINED VARIABLES*/
#define PORT 3946
#define MSG_SIZE 4096
#define NAME_SIZE 20
/*IPADDRESSES*/
//#define IP_ADRESA "192.168.1.3"
//#define IP_ADRESA "109.93.49.194"
#define IP_ADRESA "192.168.1.5"
//#define IP_ADRESA "192.168.1.2"
/*ENCRYPTION KEYS*/
#define KEY 13
#define X_KEY 0b01010101
#define M_KEY "Ol4pEdHZSAUPxiTOBX6BfvYd3VSpUYEYuHhILnKY2jNafnmUi29ZMC1ucnnX2eY8pnkPqBVzu3f7dKGeLF2p4e8GP4mxG54x8zZNjKaC5UT1zZkR5Krk7Y15QB0f6PKGKiVoq3By5PUMkztjC0MytwSTyle4nGfPxg1D76YUi3zhO5Je1iqPNjTvpbJSSwLo2G92mcQJuLVQdRxhgX0TAZUPWuD3TWpZi8v2cD84EGpoIFJYhDLaIZa2LzmVB5u2utrjGGHVAxKP2s8dLNFTPLvaxgtwggogH7t1SKrPryIh3qjBPaxwZWw59yQVctMMqsX599olKNVcgrdPbCTVnHDnHKYOfRB7MAF5APjsxkdfJogi4qY1ukMwLzOrQIIW0QSPxKcMNkusRLnL0Nn9kGjUSHBgVE2LUBZkf891u9sAy5dHDnxAXSJSyxNM3Qb2gI1h61qeDnbW4TJ33p7oQAKAMBjw25Gwt8Da87VvhWAptlHh7DdEEVn7RmqBniNdU5zsXk9rqQhGAX5gTQHdbuokgXddbmKxb7B9QxpLioj0E8JvftoZKi9w1yFnfWHAOv5wOCuFpDjNvLwIhYj8e7nHqZlmYJ1QwGlVdgoEEcL2VCdURp8oPcHs3vfELJRDAhWDkW524XZDxICwFrCRwDrhwJfGOXeK5J6skLysw4n7mFvOHL1VtCUdnLKfgaxOs9vnSz3GeszrrHP9DiEqyOeLgUkLTkG1n8DQxUmgEiBeDluFdLxSl2ntjn2rF6YDP1AsVha6B0NT5Tv9YKSuMnzdr0L6TSRFxBkMqHyd3Qhjq3VSQVAcylqlR20VZX5WsgOMzuEo1yvjHlMkoBz7PtI6jXQ5GdrlASqQ1OCh1RRCSbNdFSmTX3wRRb70ksYv7JpiZwDdkImddR74BzsSZCY1hnhPxT048MI9VlbLIZOH8aoqYBMeXuWMtbzLhetIJagUxw9opeAUAIGn3PZ5nRxIoyx4XlQlEldAF06d92dt9hjy4v538OM4pcn0BheEkVaWWYygNDc5m2JGV1mAEuZ4mZ0QbiedP4Z7eKClXfAKk1czQ1X3yQNgFQiQmRkUMFBGMZCREzlCMcNNgzqFGPOod15KSDBbYdtIoAXHkU56ikla2TUGnYDSTIJAXAnEzWnGZkwVj3HZoKihpXWvVvItWOk0ILM8VMJ7tn9JK09XhuKqs9oEJ5ALUbgYlPL6E4fKB3AH5Gac0lDZMWni2KxXmgspmQhPyw3b7io3OhtFFUmBFQNHdz56ojAi67bMXtv78yVvfef7efcVOGGBDEKuMf20zN0LXOFpHbkplzMfl8B14jjP5Ebzzm07qw9MbMpiesGOWfiDsMjc4rp29Ph6dFjLZe4cBgaC8E953O1Hy0u50WVh4CmRxnQo03dInNJBkG51WKciskdsqPzjklHmRGxMxe8MSNf8Lg8Vw8jsYLxgq2rHdwM1lF7KnJWe0oaZR5wk6V7lRRNRZSVagnDZUrdwkxVkS6SsuKjkdhpRlXqMfqkC1C5kSwuVRcD3Bm9xuLVIybMQBEij5vWggykhc7aLZqZG35a6dlD8HpyUXWZQpTx32kHNMk14kypSO6E954ilJqauDCOaEJHSWbSFBssHDqOkJiOWLfZ26JHEdTI9PHyxFkl14lzMUsOJVjZx2DzGdHKzfiLTLLFwIBo7JjmGOjSkJK3HJW9SSqa9Vo7XTOZNmNCjTkax04y04lEEu8ROAUeW9CFX7HDZDTMVhNtcySjw2YNwrc4cGYUKOp4KLV2OpYBBvidLKtRQJE3Qz7PJq1G4qLZAtNIDSC4OG0n6AL9Qf79RheKGhC70PbAj6ConsVEYRVhykp358DmrNCFLgqRNnzecI5iD1FelH8H7sr1lvoPf4h6f3k3nAzZ4mO3P9v09yUtmM1PFW7x4Ek0esPncTy3BggcQmrDDnQrqb5ZQp0Lv5vhNtZ2V4GguHFxT9zjvMdgXAJHXVEqoxbJo4KPT3AezHEKcjgcdRWKODpnTdcjqse3LxYlbNN1VEc8y5TlcqkjA3dElnD7dngAG37yvROlORGHkXb90PDD4NsE5JEoiJbgFEtDZXxaoOaOwqKLLZRHTYpqGz0Lo373DOTqZD5eOI9txk4jzxZJPciULy4lEUDx7dzkw3Tdoiq13pPRA8K9NJTXVBchBueC3WdtRbsu40UIipQeCXANx6KhvXquEiCo6QoI3ZsrbdX0o9Ls61R3p3Pl8ECszZjjaB5X9mIMkGPUjnVUWE1Jtsum6tFuboyjuoDLmqaNXHzU0L0V8riHNdIL08OQhG6mm1sCn2zHDlApwuyaxMv7QxPwmJauDba9pGy9mkNx9EXx9MQQBUQPVNts57P5t7HIDi4qdV2r6qllJvvbejthisUg0Yi4Dwbb8uy5WmbSwqOxadwyAviH6Q51FFWBTIrEwLlKYBB9TCFuiSCbmotowbx3MdnLuNsSe3CbSEOBfvbJXO9c4CxR3fOeCvVCH6OnP7VbyXZyJ1qe8OYJIl98KrCzpWEUC0OM20Gg3XLy8QuCbXmmoTSpxayngAtJeCq10d8xE4bgOHAmMOMhXLfT4PQpzRtt7xVykNvsZZQEC8AZy4rfl3Tu17Y09kDbbkBRUn4aG9lw300MzgxCl01BQb2GUtE6L3qKnyQ2GwCSmSlXqTnPhqNDXFE1lPCA3RCnHXRgeq4qH0Ebidl7BguKAgBiY2NX2GnLqWzniYgY5YcOK2Kdp8WVhgMHS6Te4BXBYW1nu9palDvK4oD5lt08AOduNsHdq2jK4IfxumlvT8TRGiQBpf4ab9cjVklYGPVc6lNY6eBg2Vq3P15SUsWrUmHcaj54BnrQOvxIptCS8xAl1sNiZXutYmUs99mjpOHzczSv6fiIQMZRCFYEnbut4tEuPxTdAIjTh2cdBZYaa5N25CTjxkXhWRekZw9KfSKWFX0Zm7GsHyv5XloN3Uts8WXzmLxSjxjr4Da8SAEbC9jXbFtCn3dEJA8Q2h549KAEYRMb7PXkcrCBzlAJ5FlTy91JGx92pL2H1H6A5XDwXlLhWSnmXk83UPN0OmaQRmXwkEGzWogMXggUie1ukDIBNj9hsnj3GDuU96PGeGtkeHx6QI2W0Vb9oIqPWBGzR33qTR8kGKj8aIusaVzTHhAMAEMqj0cw1YaXMoSD2HH0RpPEEashcDzpeicW8C1seR3czosk8tEJyxfS7pREDATf7SlmFF1HAzXNWB6eI5uI7JBbRGMgPFaH0DSc0pP68Catfhq5NAN4sHzzXY5CkpFhIOynqLZ0IGks2eCRFG9QWwkBz3jJgRHjEfQisJMMz6gatqecMmqRDSuZ6HelhECzP5T1X0EXM2j371sqGk6hAHWnPtVox3vZFHEX0EooL8ceHq78d8iogxKFALf6I3iARp1tqOykmSAKtaLjj7wbChYscFGDoaUqqGxe4vBDst2bUiXePfRWdsmrW78BeCjpK9Rh5bL3CNR2v6J07p3eEtk2eNbVXSHkCaX6EQIQAedNk7PunR5hnUl8BynxBjeIRwZfPrwoFLbUzY5GXvPQoi2wbTvXuOQbSrj5mCeAO7F9087XJ8zLVwbVZVp8d5UuLY9aiqzmb48d2SXUUmaWO1YJy01jhWclVh0ECXSnTGxugsl1vCgdEVZCd9HTp9qCinmq37rGKSWaDT42PmLpi7DNODxlvMyUsbyBOCi4qHDpa5lH2d7dP6hR3usU4GS8Wo77ceNTIwFfWTidRfhT4MUDpiNK9vst2TaQwP4QmWZnB80OwseG2b4X22oS55SQceshse3uC5sVLYg3ecIXyjRKufh9MnZCtSS2XH9wEir7KU5fpgS9ykJFdUDz9PWC8MdyzXDBNjXUykOqL8D1veaBGhVviLNWPr5i8FuFOoFogfXfWbuCR70Vjp8HVCrakl1Y76pqQfug6M5u43HHHkXEX9ZfAtyIIr0jdNUIEqYTQKb9w5HOTiLAMiN5pKWkWPFiI58q1MUhGZAV6QUuzanIwvfjsCzg4GdF1MgJVTpMWu7wLxzaHkZIxhHlwQ7WD4swfYgiuinAB7jiPNkRhni4dwjYBrCcMNjcd"
/*DATA FILES*/
#define NICK_DAT "../data/nick.dat"
#define LOGS_DAT "../data/logs.dat"
#define MESSAGES_DAT "../data/messages.dat"
#define HIDDEN_DAT "/var/tmp/hidden_mes.dat"
#define ACTIVE_DAT "../data/active.dat"
#define BAN_DAT "../data/ban.dat"
/*TERMINAL COLOR CODES*/
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define YELLOW  "\033[32m"      /* Yellow */
#define GREEN   "\033[33m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLD	"\033[1m"
#define COLORONE    "\033[1m\033[30m"      /* Bold Black */
#define COLORUSER   "\033[1m\033[31m"      /* Bold Red */
#define COLOREXEC   "\033[1m\033[32m"      /* Bold Yellow */
#define BOLDGREEN   "\033[1m\033[33m"      /* Bold Green */
#define COLORTILDA  "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#endif
