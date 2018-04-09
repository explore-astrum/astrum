pragma solidity ^0.4.17;

import "./Ownable.sol";

contract Land is Ownable {

    mapping (string => address) ownership;
    mapping (string => uint) sale;

    event SaleSet(string key, uint amount);
    event Transferred(string key, address from, address to);

    function owner_of(string _key) public view returns (address _owner) {
        _owner = ownership[_key];
        if (_owner == address(0)) {
            _owner = owner;
        }
    }

    modifier only_owner_of(string _key) {
        require(owner_of(_key) == msg.sender);
        _;
    }

    function set_sale(string _key, uint _amount) public only_owner_of(_key) {
        SaleSet(_key, _amount);
        sale[_key] = _amount;
    }

    function get_sale(string _key) public view returns (uint) {
        return sale[_key];
    }

    function buy(string _key) public {
        // TODO: Transfer token
    }

    function transfer(string _key, address _to) public only_owner_of(_key) {
        transfer(_key, msg.sender, _to);
    }

    function transfer(string _key, address _from, address _to) private {
        owner = owner_of(_key);
        require(owner == _from);
        ownership[_key] = _to;
        Transferred(_key, _from, _to);
    }

}