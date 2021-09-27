import * as React from "react"

class ChallengePage extends React.Component {
  constructor(props) {
    super(props);
  }

  render() {
    return (
      <div classname="challenge-page">
        <h1>Challenge #{this.props.index}: {this.props.name}</h1>
        <p>Challenge contents goes here</p>
        <div><p>Linux terminal goes here</p></div>
      </div>
    );
  }
}

const IndexPage = () => {
  return(
    <ChallengePage
      index="0"
      name="hello"
    />
  )
}

export default IndexPage;