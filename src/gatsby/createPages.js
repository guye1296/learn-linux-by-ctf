'use strict';


const path = require("path");


module.exports = async ({graphql, actions}) => {
    const {createPage} = actions;
    const challengePage = path.resolve("./src/templates/challenge.js");

    // Get all the challenges markdown files to parse
    const allChallengesResult = await graphql(`
      {
        allMarkdownRemark {
          edges {
            node {
              fields {
                slug
              }
            }
          }
        }
      }
    `);

    if (allChallengesResult.errors) {
        console.error(allChallengesResult.errors);
        throw allChallengesResult.errors;
    }

    const challengesEdges = allChallengesResult.data.allMarkdownRemark.edges;

    // TODO: sort challenges?

    // Create challenge page for each result
    challengesEdges.forEach((edge, index) => {
        const slug = edge.node.fields.slug;
        createPage({
            path: slug,
            component: challengePage,
            context: {
                slug: slug,
            },
        });
    });
};
