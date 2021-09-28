import React from "react";
import { graphql } from "gatsby";
import ChallengeLayout from "../components/challenge_layout"


class Challenge extends React.Component {
    render() {
        return (
          <ChallengeLayout
              config={this.props.data.toml.general}
              description={this.props.data.markdownRemark.html}
          />
        );
    }
};


// Each page is identified by a slug
// The slug is attached to both toml and md nodes
const pageQuery = graphql`
    query challengeContentsQuery($slug: String!) {
        markdownRemark(fields: {slug: {eq: $slug}}) {
            html
        }
        toml(fields: {slug: {eq: $slug}}) {
            general {
                challenge_name
              }
        }
    }
`;


export {pageQuery};
export default Challenge;