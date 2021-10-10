import * as React from "react"
import { useStaticQuery, graphql } from "gatsby"
import ChallengeList from "./challenge_list"
import V86Machine from "./v86_machine"


const ChallengeLayout = ({config, description }) => {
    const titleQuery = useStaticQuery(graphql`
    query {
      site {
        siteMetadata {
          title
        }
      }
    }
    `)

    const challengeName = config.challenge_name;

    return (
      <div className="challenge-page">
        <ChallengeList/>
        <title>{titleQuery.site.siteMetadata.title} | {challengeName} </title>
        <h1>{challengeName}</h1>
        <div
          className="blog-post-content"
          dangerouslySetInnerHTML={{ __html: description }}
        />
        <V86Machine/>
      </div>
    );
}


export default ChallengeLayout;