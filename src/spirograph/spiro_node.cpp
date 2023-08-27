#include "spiro_node.hpp"



void SpirographPen::tick(const CircleTransform& parent, glm::vec2 parent_cart_pos) {
    const glm::vec2 cart_pos = transform.position.to_cartesian(parent_cart_pos);
    const float stroke_width = transform.radius;

    transform.position.angle = parent.rotation;

    strip_renderer->get_strip().push_point(cart_pos, stroke_width, glm::vec4(color, 1.0f));
}

void SpirographPen::render() const {
    strip_renderer->render();
}


void SpirographNode::render() const {
    if (pen)
        pen->render();

    for (const auto& child : children)
        child.render();
}

void SpirographNode::tick(float delta, CircleTransform& parent_transform, glm::vec2 parent_cart_pos) {
    const glm::vec2 cart_pos = transform.position.to_cartesian(parent_cart_pos);

    if (pen)
        pen->tick(transform, cart_pos);

    const float angle_delta = delta * rotation_speed;
    transform.rotate(parent_transform, angle_delta);

    for (auto& child : children)
        child.tick(delta, transform, cart_pos);
}


void Spirograph::render() const {
    for (const auto& child : children)
        child.render();
}

void Spirograph::tick(float delta) {
    for (auto& child : children)
        child.tick(delta, transform, position);
}


SpirographPen Pen::build(const CircleTransform& parent_transform) const {
    const PolarCoords position(0.0f, parent_transform.radius * radius_mult);
    const CircleTransform transform(position, stroke_width, 0.0f);

    return SpirographPen(transform, color);
}

Pen& Pen::set_position(float new_radius_mult) {
    radius_mult = new_radius_mult;

    return *this;
}

Pen& Pen::set_stroke_width(float new_stroke_width) {
    stroke_width = new_stroke_width;

    return *this;
}

Pen& Pen::set_color(glm::vec3 new_color) {
    color = new_color;

    return *this;
}


SpiroNode SpiroNode::root() {
    return SpiroNode(Attachment::Inside);
}

std::unique_ptr<Spirograph> SpiroNode::build(glm::vec2 position) {
    return std::unique_ptr<Spirograph>(new Spirograph(position, radius, build_node_children()));
}

SpiroNode SpiroNode::inside() {
    return SpiroNode(Attachment::Inside);
}

SpiroNode SpiroNode::outside() {
    return SpiroNode(Attachment::Outside);
}

SpiroNode& SpiroNode::attach(SpiroNode child) {
    children.push_back(child);

    return *this;
}

SpiroNode& SpiroNode::set_pen(Pen new_pen) {
    pen = new_pen;

    return *this;
}

SpiroNode& SpiroNode::set_radius(float new_radius) {
    radius = new_radius;

    return *this;
}

SpiroNode& SpiroNode::set_rotation_speed(float new_rotation_speed) {
    rotation_speed = new_rotation_speed;

    return *this;
}

SpiroNode& SpiroNode::set_starting_angle(float new_starting_angle) {
    starting_angle = new_starting_angle;

    return *this;
}

std::vector<SpirographNode> SpiroNode::build_node_children() const {
    std::vector<SpirographNode> node_children;
    for (const SpiroNode& child : children)
        node_children.push_back(child.build_node(*this));

    return node_children;
}

SpirographNode SpiroNode::build_node(const SpiroNode& parent) const {
    float position_radius = attachment == Attachment::Inside ? parent.radius - radius : parent.radius + radius;

    const PolarCoords position(starting_angle, position_radius);
    const CircleTransform transform(position, radius, starting_rotation);

    std::optional<SpirographPen> node_pen = pen ? std::optional(pen->build(transform)) : std::nullopt;

    return SpirographNode(transform, rotation_speed, std::move(node_pen), build_node_children());
}
