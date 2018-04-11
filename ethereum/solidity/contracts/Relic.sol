pragma solidity ^0.4.19;

import "./Asset.sol";

contract Relic is Asset {

    event Combined(string _parent, string _child, address from);
    event Exploded(string _parent, string _child, address from);
    mapping (string => mapping (string => bool)) children;
    mapping (string => string) parents;

    function combine(string _parent, string _child) public only_owner_of(_parent) only_owner_of(_child) {
        require(!has_parent(_child));
        children[_parent][_child] = true;
        parents[_child] = _parent;
        burn(_child);
        Combined(_parent, _child, msg.sender);
    }

    function explode(string _parent, string _child) public only_owner_of(_parent) only_owner_of(_child) {
        require(is_child(_parent, _child));
        delete children[_parent][_child];
        delete parents[_child];
        create(_child, msg.sender);
        Exploded(_parent, _child, msg.sender);
    }

    function has_parent(string _child) public view returns (bool) {
        return bytes(parent_of(_child)).length != 0;
    }

    function parent_of(string _child) public view returns (string) {
        return parents[_child];
    }

    function is_child(string _parent, string _child) public view returns (bool) {
        return children[_parent][_child]
    }

    

}