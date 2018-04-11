const Land = artifacts.require('Land')
const Relic = artifacts.require('Relic')

module.exports = function (deployer) {
    deployer.deploy(Land).then(console.log)
    deployer.deploy(Relic).then(console.log)
}