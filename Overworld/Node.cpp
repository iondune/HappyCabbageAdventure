void Node::isCurrent() {
   return curNode;
}

void Node::setPosition(SVector2 position) {
   Position.X = position.x;
   Position.Y = position.y;
}

void Node::getPosition() {
   return Position;
}
