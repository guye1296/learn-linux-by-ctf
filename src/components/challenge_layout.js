import * as React from "react"
import { useStaticQuery, graphql } from "gatsby"

const ChallengeLayout = ({ index, name }) => {
    const data = useStaticQuery(graphql`
    query {
      site {
        siteMetadata {
          title
        }
      }
    }
  `)

    return (
      <div classname="challenge-page">
        <title>{data.site.siteMetadata.title}| #{index} </title>
        <h1>Challenge #{index}: {name}</h1>
        <p>Challenge contents goes here</p>
        <div><p>Linux terminal goes here</p></div>
      </div>
    );
}


export default ChallengeLayout;