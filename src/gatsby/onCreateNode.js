'use strict';


const path = require('path');


module.exports = exports.onCreateNode = ({node, actions, getNode}) => {
    const {createNodeField} = actions;

    switch (node.internal.type) {
        case 'MarkdownRemark':
        case 'TOML':
            const slug = `/${path.relative('.', getNode(node.parent).dir)}.html`;

            createNodeField({
                node, 
                name: 'slug',
                value: slug,
            });

            return;
    }
}