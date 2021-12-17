import * as React from "react"
import { useStaticQuery, graphql, Link } from "gatsby"


const ChallengeList = () => {
    const challengesListQuery = useStaticQuery(graphql`
    query {
        allToml {
          edges {
            node {
              fields {
                slug
              }
              general {
                challenge_name
              }
            }
          }
        }
      }
    `);

    return (
      <ul>
        {challengesListQuery.allToml.edges.map((item, idx) => 
          <li key={idx}><Link to={`${item.node.fields.slug}`}>{item.node.general.challenge_name}</Link></li>
        )}
      </ul>
    );
}


export default ChallengeList;