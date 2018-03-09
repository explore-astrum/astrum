import os
import threading
import json
from time import sleep

from logzero import logger
from twisted.internet import reactor, task

from neo.contrib.smartcontract import SmartContract
from neo.Network.NodeLeader import NodeLeader
from neo.Core.Blockchain import Blockchain
from neo.Implementations.Blockchains.LevelDB.LevelDBBlockchain import LevelDBBlockchain
from neo.Settings import settings

from google.cloud import pubsub_v1

from neocore.Cryptography import Crypto

smart_contract = SmartContract(os.environ["CONTRACT"])
publisher = pubsub_v1.PublisherClient()
topic = publisher.topic_path("astrum-world", "neo")

def parse_arg(input):
    if isinstance(input, int):
        return input
    try:
        return input.decode('utf-8')
    except:
        pass
    if len(input) == 20:
        try:
            return Crypto.scripthash_to_address(input)
        except:
            pass
    return input

@smart_contract.on_notify
def sc_notify(event):
    if not len(event.event_payload):
        return
    args = list(map(parse_arg, event.event_payload))
    logger.info(args)
    publisher.publish(topic, json.dumps(args).encode("utf-8"))


def log_block():
    while True:
        logger.info("Block %s / %s", str(Blockchain.Default().Height), str(Blockchain.Default().HeaderHeight))
        sleep(15)


def main():
    # Setup the blockchain
    # settings.setup('./neo-python/protocol.coz.json')
    blockchain = LevelDBBlockchain(settings.LEVELDB_PATH)
    Blockchain.RegisterBlockchain(blockchain)
    dbloop = task.LoopingCall(Blockchain.Default().PersistBlocks)
    dbloop.start(.1)
    NodeLeader.Instance().Start()


    # Disable smart contract events for external smart contracts
    settings.set_log_smart_contract_events(False)

    # Start a thread with custom code
    d = threading.Thread(target=log_block)
    d.setDaemon(True)  # daemonizing the thread will kill it when the main thread is quit
    d.start()

    # Run all the things (blocking call)
    logger.info("Everything setup and running. Waiting for events...")
    reactor.run()
    logger.info("Shutting down.")


if __name__ == "__main__":
    main()
