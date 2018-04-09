pragma solidity ^0.4.17;

contract Land {
    function encode(uint x, uint y) public pure returns (uint256) {
        return uint256(keccak256(x, y));
    }

}